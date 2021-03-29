import os
import json
from http.server import BaseHTTPRequestHandler, HTTPServer

host_port = 8000

pin1 = 0

class Server(BaseHTTPRequestHandler):

    def do_HEAD(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()

    def do_GET(self):
        if self.path == "/":
            self.path = "index.html"
        else:
            self.path = self.path[1:]

        file = open(self.path, mode='rb')
        data = file.read()
        file.close()
        self.do_HEAD()
        self.wfile.write(data)

    def do_POST(self):
        # Get the size of data
        content_length = int(self.headers['Content-Length'])
        # Get the data
        post_data = self.rfile.read(content_length).decode("utf-8")

        print("Data: ", post_data)


if __name__ == '__main__':
    http_server = HTTPServer(('', host_port), Server)
    print("Server started on port: %s" % host_port)

    try:
        http_server.serve_forever()
    except KeyboardInterrupt:
        http_server.server_close()