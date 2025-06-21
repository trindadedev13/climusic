require "rbconfig"
require "fileutils"
require "./.scripts/color.rb"

def run(cmd)
  system(cmd) or abort(red("Failed to run #{cmd}"))
end

asan = false
run_program = false

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
      when "--asan", "-as"
        asan = true
      when "--run", "-r"
        run_program = true
      else
        puts "Invalid arg: #{arg}"
    end
  end
end

FileUtils.mkdir_p("build")
Dir.chdir("build") do
  cmake_command = "cmake " + (asan ? "-DASAN=ON .." : "..")
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