#!/usr/bin/python

import subprocess
import sys
import os
import cgi

print "Content-type: text/html"
print
print "<title>Test CGI</title>"

print """<!DOCTYPE html>
<head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
</head>
"""



form = cgi.FieldStorage()
form = cgi.FieldStorage()
picture_from_form = form.getvalue("picture")

if str(picture_from_form) != 'None':
        #print "The user entered", picture_from_form
        f = open("rgb_strip.conf", "w")
        f.write(str(picture_from_form))
        f.close()

f = open("rgb_strip.conf", "r")
print 'Last selection is:<b>', f.read(),'</b>'
f.close()

root_folder = "pictures"

print '<br><br>'
print "<form>"
for files in os.listdir(root_folder):
        #print files
        if files.endswith(".bmp"):
                #print files, '<br>'
                print '<a href="?picture='+files+'">'
                #print '<img src="../'+files+'" /> <br>'
                print '<img src="../pictures/'+files+'" /> <br>'
                print "</a>"
                print "<br>"

print "<form>"
