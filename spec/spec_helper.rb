require 'rubygems'
gem 'rspec', '~> 1.1'

begin
  require 'ruby-debug'
rescue LoadError
  puts 'ruby-debug is not available. Good luck debugging'
end

require 'spec'

require File.join(File.dirname(__FILE__), *%w(.. lib ruby-mapnik))

