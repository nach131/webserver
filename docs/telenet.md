telnet example.com 8080


POST /web HTTP/1.1
Host: example.com
Content-Type: multipart/form-data; boundary=---------------------------boundary123456789
Content-Length: 197
Connection: close

---------------------------boundary123456789
Content-Disposition: form-data; name="file"; filename="file.txt"
Content-Type: text/plain

Esto es lo que hay dentro del fichero
---------------------------boundary123456789--
