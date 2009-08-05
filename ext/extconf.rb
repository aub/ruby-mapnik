# Loads mkmf which is used to make makefiles for Ruby extensions
require 'mkmf'

# Give it a name
extension_name = 'mapnik'

# The destination
dir_config(extension_name)

dir_config('boost')
dir_config('mapnik')
dir_config('freetype')
dir_config('opt')

have_library('mapnik')
have_library('boost_thread-mt')

# Do the work
create_makefile(extension_name)

# ruby extconf.rb --with_mapnik_include=/usr/local/include/mapnik --with_boost_include=/usr/local/include/boost-1_37 --with_freetype_include=/opt/local/include/freetype2 --with_opt_include=/opt/local/include
#
# g++ -g -I/usr/local/include/mapnik -I/opt/local/include -I/opt/local/include/freetype2 -I../../agg/include -I/usr/local/include/boost-1_37 -L/usr/local/lib -L/opt/local/lib -lmapnik  -lboost_thread-mt -licuuc cali.cpp -o cali
