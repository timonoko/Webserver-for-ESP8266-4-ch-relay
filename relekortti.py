
print('toimiiko')

try:
  import usocket as socket
except:
  import socket

import network

import time

releet = [0,0,0,0,0]

from machine import UART
uart = UART(0, 115200)
uart.init(115200, bits=8, parity=None, stop=1, timeout=1000)
uart.write(b'Uart toimii 5')

#def rele(r,o): # dammit
#    releet[r]=o
#    n=10*r+o
#    if n==10: s=b'\xA0\x01\x00\xA1'
#    if n==11: s=b'\xA0\x01\x01\xA2'
#    if n==20: s=b'\xA0\x02\x00\xA2'
#    if n==21: s=b'\xA0\x02\x01\xA3'
#    if n==30: s=b'\xA0\x03\x00\xA3'
#    if n==31: s=b'\xA0\x03\x01\xA4'
#    if n==40: s=b'\xA0\x04\x00\xA4'
#    if n==41: s=b'\xA0\x04\x01\xA5'
#    uart.write(s) 

def rele(r,o): # I finally made it
    releet[r]=o
    uart.write(b'%c%c%c%c'%(0xA0,r,o,0xA0+o+r))
    
def web_page():
  html = """
     <html><head> 
     <title>ESP Web Server</title>
     <meta name="viewport" content="width=device-width, initial-scale=1">
     <link rel="icon" href="data:,">
     <style>html{font-family: Helvetica; display:inline-block; margin: 0px auto; text-align: center;}
  h1{color: #0F3376; padding: 2vh;}p{font-size: 1.5rem;}.button{display: inline-block; background-color: #e7bd3b; border: none; 
  border-radius: 4px; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
  .button2{background-color: #4286f4;}</style>
     </head>
      <body>
     <h1>RELEET</h1> 
    <p><strong>""" + str(releet[1]) + """</strong>
        <a href="/r1on"> <button class="button">ON</button></a>
        <a href="/r1off"> <button class="button button2">OFF</button></a></p>
    <p><strong>""" + str(releet[2]) + """</strong>
        <a href="/r2on"> <button class="button">ON</button></a>
        <a href="/r2off"> <button class="button button2">OFF</button></a></p>
    <p><strong>""" + str(releet[3]) + """</strong>
        <a href="/r3on"> <button class="button">ON</button></a>
        <a href="/r3off"> <button class="button button2">OFF</button></a></p>
    <p><strong>""" + str(releet[4]) + """</strong>
        <a href="/r4on"> <button class="button">ON</button></a>
        <a href="/r4off"> <button class="button button2">OFF</button></a></p>
    <p> <a href="/exit"> <button class="button">EXIT</button></a></p>
     </body>
   </html>"""
  return html

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('', 80))
s.listen(5)

while True:
  conn, addr = s.accept()
  request = conn.recv(1024)
  request = str(request)
  for r in range(1,5):
    if request.find('/r'+str(r)+'on') == 6:
       rele(r,1)
    if request.find('/r'+str(r)+'off') == 6:
       rele(r,0)
  if request.find('/exit') == 6:
      import webrepl
      webrepl.start()
  response = web_page()
  conn.send('HTTP/1.1 200 OK\n')
  conn.send('Content-Type: text/html\n')
  conn.send('Connection: close\n\n')
  conn.sendall(response)
  conn.close()
