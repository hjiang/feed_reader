task :default => :build

desc "Clean up build artifacts"
task :clean do
  sh 'rm -rf build'  # Remove compilation targets and cmake output.
end

directory "build/arm"
directory "build/x86"

file "build/x86/Makefile" => "build/x86" do
  sh 'cd build/x86 && cmake ../..'
end

file "build/arm/Makefile" => ["build/arm", :arm_env] do
  sh 'cd build/arm && cmake -DBUILD_FOR_ARM:BOOL=ON ../..'
end

namespace :config do
  task :env do
    path = ENV['PATH']
    ENV['PATH'] = "/opt/onyx/arm/bin:#{path}"
  end

  task :x86 => "build/x86" do
    sh 'cd build/x86 && cmake ../..'
  end

  task :arm => ["build/arm", :env] do
    sh 'cd build/arm && cmake -DBUILD_FOR_ARM:BOOL=ON ../..'
  end
end

namespace :build do
  [:x86, :arm].each do |arch|
    desc "Build feed_reader (#{arch})"
    task arch => "config:#{arch}" do
      sh "cd build/#{arch} && make -j2 "
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
