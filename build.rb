require "rbconfig"
require "fileutils"
require "./.scripts/color.rb"

def run(cmd)
  system(cmd) or abort(red("Failed to run #{cmd}"))
end

def help()
  puts "CLIMusic Build Script"
  puts "--run  or -r   | Run after build."
  puts "--asan or -as  | Build with Address Sanitizer (Debug purposes)."
  puts "--help or -h   | Shows helps."
  exit 1
end

run_program = false
asan = false

os = case RbConfig::CONFIG["host_os"]
       when /mswin|mingw|cygwin/ then :window
       when /linux/
         if ENV["PREFIX"]&.include?("/data/data/com.termux")
           :termux
         else
           :linux
         end
       when /darwin/             then :mac
       else :unknow
     end

if ARGV.length >= 1
  ARGV.each do |arg|
    case arg
      when "--run", "-r"
        run_program = true
      when "--asan", "-as"
        asan = true
      when "--help", "-h"
        help()
      else
        puts "Invalid arg: #{arg}"
    end
  end
end

FileUtils.mkdir_p("build")
Dir.chdir("build") do
  cmake_command = "cmake " + (asan ? "-DASAN=ON .." : "-DASAN=OFF ..")
  case os
    when :windows
      # Windows
      run("#{cmake_command} -G \"MinGW Makefiles\"")
      run("mingw32-make")
    else
      # Linux
      run(cmake_command)
      run("make")
  end
end

if run_program
  home = ENV["HOME"]

  executable = case os
    when :windows
      File.join("build", "climusic.exe")
    when :termux
      File.join(home, "temp", "climusic", "main")
    else
      File.join("build", "climusic")
  end

  command = case os
    when :windows
      exe
    else
      File.join(home, "temp", "climusic", "main")
    end

  if os == :termux
    FileUtils.mkdir_p(File.join(home, "temp", "climusic"))
    FileUtils.cp(File.join("build", "climusic"), executable)
  end

  if [:linux, :mac, :termux].include?(os)
    FileUtils.chmod("+x", executable)
  end

  puts green("Running at #{os}...")
  run(command)
end