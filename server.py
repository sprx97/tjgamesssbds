import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('', 50007))
s.listen(1)
conn, addr = s.accept()
print 'Connected by', addr
while 1:
	data = conn.recv(256)
	print 'Received', data
