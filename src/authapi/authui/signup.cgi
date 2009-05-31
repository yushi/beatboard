#!/usr/bin/env python3.0
# #!/usr/bin/env python
# -*- coding: utf-8 -*-

import cgi
from urllib.request import urlopen
from urllib.parse import urlencode

def header():
    print("Content-type: text/html\r\n")

def fieldstorage_to_dict(form):
    field = {}
    for key in form.keys():
        field[key] = form[key].value
    return field

def redirect_api(form):
    url = 'http://localhost:8082/auth'
    field = fieldstorage_to_dict(form)
    params = urlencode(field)
    f = urlopen(url + '?' + params)
    data = f.read()
    print(data.decode('utf-8'))

def validate_field(form, keys):
    return all([key in form for key in keys])

def main():
    header()
    keys = ["n", "p", "m"]
    form = cgi.FieldStorage()
    if (validate_field(form, keys)):
        #print("correct query %s" % [form[key].value for key in keys])
        #print()
        redirect_api(form)
    else:
        print("suspicious query received")

main()
