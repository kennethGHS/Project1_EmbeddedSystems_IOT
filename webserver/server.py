from http.server import BaseHTTPRequestHandler, HTTPServer

host_port = 8000


class Server(BaseHTTPRequestHandler):

    def do_HEAD(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()

    def do_GET(self):
        if self.path == "/":
            self.path = "index.html"
        elif self.path == "/api/sensors":
            sensors = open("sensors.json", mode="rb")
            data = sensors.read()
            sensors.close()
            self.do_HEAD()
            self.wfile.write(data)
        elif self.path == "/api/picture":
            picture = open("picture.jpg", mode="rb")
            data = picture.read()
            picture.close()
            self.do_HEAD()
            self.wfile.write(data)
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
        post_data = self.rfile.read(content_length)

        if self.path == "/api/sensors":
            sensors = open("sensors.json", mode="w")
            sensors.write(post_data.decode("utf-8"))
            sensors.close()
        elif self.path == "/api/picture":
            picture = open("picture.jpg", mode="wb")
            picture.write(post_data)
            picture.close()

        self.do_HEAD()


if __name__ == '__main__':
    http_server = HTTPServer(('', host_port), Server)
    print("Server started on port: %s" % host_port)

    try:
        http_server.serve_forever()
    except KeyboardInterrupt:
        http_server.server_close()
