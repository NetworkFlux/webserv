#!/usr/bin/python

import cgi

# Get the input from the form
form = cgi.FieldStorage()
string1 = form.getvalue('string1')
string2 = form.getvalue('string2')

# Concatenate the strings
result = string1 + string2

# Print the HTTP header
print("Content-type: text/html\n")

# Print the result
print(result)
