#Takes in a dir of all the other projects, collects in zip and emails
# Zip also contains summary files (XML files displayed with XSLT) and a HTML index

import os
import subprocess
import re
import lxml.html
from lxml import etree
import shutil
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email import encoders

cwd = os.getcwd()
path = os.path.join(cwd, 'csc344')

for root, dirs, files in os.walk(path, topdown=False):
    os.chdir(root)

    #Number project -> adding the xslt removes this capability
    basename = os.path.basename(root)
    treeroot = etree.Element("Project")
    treeroot.addprevious(etree.ProcessingInstruction('xml-stylesheet', 'type="text/xsl" href="../stylesheet.xsl"'))

    for file in files:
        #Name of project file
        filename, file_extension = os.path.splitext(file)
        if file_extension != '.xml' and file_extension != '.xsl' and file_extension != '.zip' and file_extension != '.html':
            
            child = etree.Element("File")
            child.text = file
            treeroot.append(child)
        
            #Address of project file 
            child2 = etree.SubElement(child, 'address')
            child2.text = str("./" + file)

            #Number of lines in the project file
            child3 = etree.SubElement(child, 'lines')
            word_count = subprocess.getoutput(['wc', '-l', file])
            child3.text = str(word_count.split()[0])

            #Identifiers
            child4 = etree.SubElement(child, 'identifiers')

            word_list = []
            with open(file) as f:
                file_input = f.read()
           
            # no strings
            # double quotes edition
            regex = r"\"(.*)\""
        
            # single quotes edition uhm.. 
            # regex2 = r"\'(.*)\'"
    
            # no comments
            # C edition /* and //
            regex3 = r"\/\/(.*)" # one lined // C comment
            regex4 = r"\/\*(.*)\*\/" # one lined /* */ C comment
            regex5 = r"\/\*([\s\S]*?)\*\/" #multilined C comment
            
            # Clj edition ;;
            regex6 = r"\;\;(.*)"

            # Python edition #
            regex7 = r"\#(.*)"

            # Filter out strings
            nostrings = re.sub(regex, "", file_input)
           # nostrings = re.sub(regex2, "", almostnostrings)
            
            # Filter out C comments
            noonelinedCcomments = re.sub(regex3, "", nostrings)
            noonelinedCstarcomments = re.sub(regex4, "", noonelinedCcomments)
            noCcomments = re.sub(regex5, "", noonelinedCstarcomments)
            
            # Filter out Clojure comments
            nocljcomments = re.sub(regex6, "", noCcomments)
            
            # Filter out Python comments
            nopythoncomments = re.sub(regex7, "", nocljcomments)

            # Filter out Prolog comments    
            # JK there were none b/c who wants to know how that works


            finalregex = r"[\w]+"
            match_list = []
            for match in re.findall(finalregex, nocljcomments):
                if match not in match_list and len(match) > 1 and not match.isdigit():
                    match_list.append(match)
               
            for item in match_list:
                child5 = etree.SubElement(child4, 'identifier')
                child5.text = str(item)
            
    if basename != "csc344":
        # Build Tree
        tree = etree.ElementTree(treeroot)
        tree.write(basename + '.xml')

# Create HTML the hard way
os.chdir(path)
with open("index.html", "w+") as html_file:
    html_file.write("<!DOCTYPE html>\n")
    html_file.write("<html>\n")
    html_file.write("<head>\n")
    html_file.write("<title>INDEX</title>\n")
    html_file.write("<h1>INDEX</h1>")
    html_file.write('<a href="' + "Project1/Project1.xml" + '">' + "Project 1" + '</a>\n') #Project 1
    html_file.write('<a href="' + "Project2/Project2.xml" + '">' + "Project 2" + '</a>\n')                                                                       #Project 2
    html_file.write('<a href="' + "Project3/Project3.xml" + '">' + "Project 3" + '</a>\n') #Project 3
    html_file.write('<a href="' + "Project4/Project4.xml" + '">' + "Project 4" + '</a>\n')
    html_file.write('<a href="' + "Project5/Project5.xml" + '">' + "Project 5" + '</a>\n')
    html_file.write("</head>\n")
    html_file.write("</html>\n")

# Create ZIP file
os.chdir("..")
shutil.make_archive("csc-344-archive", 'zip', path)

# Send email
receiveremail = input("Enter an email to send the documents to: ")

session = smtplib.SMTP('smtp.gmail.com', 587)
session.starttls()
session.login("", "") #useremail, userapppass
message = MIMEMultipart()
message['Subject'] = "CSC-344 Assignments"
message['From'] = "@gmail.com" #useremail
message['To'] = receiveremail
body = "These are the assignments for the semester. Thanks!"
message.attach(MIMEText(body, 'plain'))

zipfile = open(os.path.join(os.getcwd(), "csc-344-archive.zip"), "rb")
attachment = MIMEBase('application', 'zip')
attachment.set_payload(zipfile.read())
zipfile.close()

encoders.encode_base64(attachment)
attachment.add_header('Content-disposition', 'attachment', filename="csc-344-archive.zip")
message.attach(attachment)

session.sendmail('rosemariafontana@gmail.com', receiveremail, message.as_string())
session.quit()
