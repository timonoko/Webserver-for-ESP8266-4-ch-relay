
print('Testing Testing')

try:
  import usocket as socket
except:
  import socket

import network,time

# print prints only to Python client
from machine import UART
uart = UART(0, 115200)
uart.init(115200, bits=8, parity=None, stop=1, timeout=1000)
uart.write(b'Uart works  #6')

releet=['-',0,0,0,0,'*']

def rele(r,o): 
    releet[r]=o
    uart.write(b'%c%c%c%c'%(0xA0,r,o,0xA0+o+r))
    time.sleep(0.2)

for x in range(1,5): rele(x,0)

def web_page():
  menu=''
  for x in range(1,6):
     menu=menu+"""
    <p> """ +str(releet[x])+ """ <a href="/r%ion"> <button class="button">ON</button></a>
     <a href="/r%ioff"> <button class="button button2">OFF</button></a></p>
    """%(x,x)
  html = """
     <html><head> 
     <title>ESP Relay Web Server</title>
     <meta name="viewport" content="width=device-width, initial-scale=1">
     <link rel="icon" href="data:,">
     <style>html{font-family: Helvetica; display:inline-block; margin: 0px auto; text-align: center;}
  h1{color: #0F3376; padding: 2vh;}p{font-size: 1.5rem;}.button{display: inline-block; background-color: #e7bd3b; border: none; 
  border-radius: 4px; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
  .button2{background-color: #4286f4;}</style>
     </head>
      <body>
     <h1>RELEET</h1> 
     """ + menu + """
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
    if request.find('/r'+str(r)+'on') == 6: rele(r,1)
    if request.find('/r'+str(r)+'off') == 6: rele(r,0)
  if request.find('/r5on') == 6:
      for x in range(1,5): rele(x,1)
  if request.find('/r5off') == 6:
      for x in range(1,5): rele(x,0)
  response = web_page()
  conn.send('HTTP/1.1 200 OK\n')
  conn.send('Content-Type: text/html\n')
  conn.send('Connection: close\n\n')
  conn.sendall(response)
  conn.close()
