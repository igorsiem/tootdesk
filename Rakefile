# TootDesk - Desktop Mastodon Client
# Copyright (C) 2018 Igor Siemienowicz
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program.  If not, see <http://www.gnu.org/licenses/>.

# This is the top-level Rakefile for TootDesk, comprising basic development
# functionality.

require 'concurrent'

# --- Config --

if Rake::Win32::windows?
    $vsyear = 2017
    puts "environment: Windows"
    puts "build using Visual Studio #{$vsyear}"
else
    puts "environment: Posix-compliant"
    puts "build using GCC"

    # TODO support clang
end

$wordsize = 32
$wordsize = 64 if ['foo'].pack("p").size == 8

puts "word size: #{$wordsize}"
puts "processors: #{Concurrent.processor_count}"
puts "---"

# --- Build / Clean ---

directory "build"

desc "clean all build artefacts"
task :clean do
    FileUtils.rm_rf "build"
end # clean task

# Assemble all the (rather complicated) options for running msbuild
#
# return:
#   A cmake config and make command strings
def assemble_msbuild_commands

    # Work out various version numbers
    #
    # TODO support other VS versions
    if $vsyear == 2017
        vernum = 15
        envnum = 14
    else
        raise "VS Studio #{$vsyear} not supported"
    end

    # Check for the environment variables we need
    env_var_name = "VS#{envnum}0COMNTOOLS"
    raise "could not find environment variable " +
        "'#{env_var_name}' - is Visual Studio 2017 installed?" \
        if ENV[env_var_name].to_s.empty?

    raise "could not find environment variable 'VCINSTALLDIR' - " +
        "have you run 'vcvars.bat'?" \
        if ENV['VCINSTALLDIR'].to_s.empty?

    platform = "Win32"
    cmake_config = "-G \"Visual Studio #{vernum} #{$vsyear} Win32\""
        
    if $wordsize == 64
        cmake_config = "-G \"Visual Studio #{vernum} " +
            "#{$vsyear} Win64\""
        platform = "x64"
    end
        
    # TODO add support for Debug builds
    make_command =
        "msbuild /m tootdesk.sln /p:Configuration=Release " +
        "/p:Platform=\"#{platform}\""
        
    return cmake_config, make_command

end # assemble_msbuild_commands method

# Work out the cmake and make build commands
task :assemble_commands do
    $cmake_config = ""
    $make_command = "make -j#{Concurrent.processor_count}"

    if Rake::Win32::windows?
        $cmake_config, $make_command = assemble_msbuild_commands
    end

    $cmake_command = "cmake #{$cmake_config} ../src"
end # assemble_commands task

desc "run cmake to produce platform-specific build files"
task :cmake => [:assemble_commands, "build"] do
    Dir.chdir "build"
    sh $cmake_command
    Dir.chdir ".."
end # cmake task

desc "build binaries"
task :bin => :cmake do
    Dir.chdir "build"
    sh $make_command
    Dir.chdir ".."
end # bin task

# --- Test ---

desc "run tests"
task :test => :bin do

    test_dir = "build/test"
    test_exe_file_name = "test-tootdesk"
    if Rake::Win32::windows?
        test_dir = "build/test/Release"
        test_exe_file_name = "test-tootdesk.exe"
    end

    cmd_line = test_dir + "/" + test_exe_file_name
    sh cmd_line

end  # test task
