#Script que requiere algun file

require 'socket'
tiempocomienzo = Process.clock_gettime(Process::CLOCK_MONOTONIC)

s = TCPSocket.new "localhost", 8989       

s.write("/tmp/testfiles/#{ARGV[0]}.c\n")

s.each_line do |line|
    #puts line
end

s.close
tiempofinal = Process.clock_gettime(Process::CLOCK_MONOTONIC)
transcurrido = tiempofinal - tiempocomienzo
puts "Tiempo transcurrido: #{transcurrido} (#{ARGV[0]})"