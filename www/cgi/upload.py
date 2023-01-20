#!/usr/bin/python3
import cgi
import os

form = cgi.FieldStorage()

# Get the file from the form
fileitem = form["file"]

# Test if the file was uploaded
if fileitem.filename:
    # Get the filename and set the destination directory
    fn = os.path.basename(fileitem.filename)
    open("www/uploads/" + fn, "wb").write(fileitem.file.read())
    message = "The file " + fn + " was uploaded successfully"
else:
    message = "No file was uploaded"

# Return HTML response
print("Content-Type: text/html\n")
print("<html>")
print("<body>")
print("<p>" + message + "</p>")
print("</body>")
print("</html>")
