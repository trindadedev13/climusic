require "fileutils"
require "./.scripts/color.rb"

def run(cmd)
  system(cmd) or abort("Failed to run #{cmd}")
end

termux = false
asan = false

if ARGV.length >= 1
  ARGV.each do |arg|
    case arg
      when "--termux", "-t"
        termux = true
      when "--asan", "-as"
        asan = true
      else
        puts "Invalid arg: #{arg}"
    end
  end
end

FileUtils.mkdir_p("build")
Dir.chdir("build") do
  if asan
    run("cmake -DASAN=ON ..")
  else
    run("cmake ..")
  end
  run("make")
end

if termux
  puts green("Running at termux...")

  home = ENV["HOME"]

  bin_path = File.join(home, "temp", "climusic")
  FileUtils.mkdir_p(bin_path)

  bin_file = File.join(bin_path, "main")

  FileUtils.cp(File.join("build", "climusic"), bin_file)
  FileUtils.chmod("+x", bin_file)

  Dir.chdir(bin_path) do
    run("./main")
  end
end