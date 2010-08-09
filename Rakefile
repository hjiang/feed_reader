task :default => :build

def has_distcc
  if ENV['DISABLE_DISTCC']
    false
  else
    system("which distcc")
  end
end

def cmake_cmd(arch)
  arm_cc = "/opt/freescale/usr/local/gcc-4.1.2-glibc-2.5-nptl-3/arm-none-linux-gnueabi/bin/arm-linux-gcc"
  arm_cxx = "/opt/freescale/usr/local/gcc-4.1.2-glibc-2.5-nptl-3/arm-none-linux-gnueabi/bin/arm-linux-g++"
  if arch == :arm and has_distcc
    "CC='distcc #{arm_cc}' CXX='distcc #{arm_cxx}' cmake "
  elsif arch == :x86 and has_distcc
    "CC='distcc gcc' CXX='distcc g++' cmake "
  else
    "CC='#{arm_cc}' CXX='#{arm_cxx}' cmake "
  end
end

def make_cmd
  if has_distcc
    "make -j12"
  else
    "make -j2"
  end
end

desc "Clean up build artifacts"
task :clean do
  sh 'rm -rf build'  # Remove compilation targets and cmake output.
end

directory "build/arm"
directory "build/x86"

namespace :config do
  task :env do
    path = ENV['PATH']
    ENV['PATH'] = "/opt/onyx/arm/bin:/opt/freescale/usr/local/gcc-4.1.2-glibc-2.5-nptl-3/arm-none-linux-gnueabi/bin/:#{path}"
  end

  task :x86 => "build/x86" do
    sh "cd build/x86 && #{cmake_cmd :x86} ../.."
  end

  task :arm => ["build/arm", :env] do
    sh "cd build/arm && #{cmake_cmd :arm} -DBUILD_FOR_ARM:BOOL=ON ../.."
  end
end

namespace :build do
  [:x86, :arm].each do |arch|
    desc "Build feed_reader (#{arch})"
    task arch => "config:#{arch}" do
      sh "cd build/#{arch} && #{make_cmd}"
    end
  end
end

desc "build:x86"
task :build => "build:x86"

desc "Run all tests."
task :test => :build do
  sh "killall -q -9 Xvfb || true"
  sh "cd build/x86 && xvfb-run -a -w 3 make test"
end
