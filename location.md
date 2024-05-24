
  /_TEST:
    allow_methods: GET
    autoindex: off
    root: ./dist

  /alias_file:
    alias: ./Docker
    allow_methods: GET POST DELETE
    autoindex: on
    index: files.html


  /dist_1/earth:
    allow_methods: DELETE POST GET
    autoindex: off
    root: ./



============

OK
  si_va.html:
    allow_methods: GET
    autoindex: off
    root: ./dist/_TEST/scrolling
OK
  /alias_web: 
    alias: ./dist/_TEST/_form
    allow_methods: GET POST DELETE
    autoindex: off
    index: form.html
OK
/:
    allow_methods: DELETE POST GET
    autoindex: off
    root: ./dist
OK
  /earth:
    allow_methods: GET
    autoindex: off
    root: ./dist_1
OK
  /files:
    allow_methods: GET POST DELETE
    autoindex: on
    index: files.html
    root: ./dist
OK
  /nada:
    allow_methods:
OK
  /other_files:
    allow_methods: GET
    autoindex: on
    index: files.html
    root: ./dist
OK
  /cgi_bin:
    allow_methods: GET
    autoindex: off
    index: photo.py
    root: /Users/nacho/Dropbox/00_42Barcelona/42Barcelona/C5/webserver
OK
  /colors:
    allow_methods: GET
    autoindex: off
    index: colors.html
    root: ./dist
OK 404 Not Found, no encuntra index
  /no_va.html:
    allow_methods: GET
    autoindex: off
    root: ./dist/_TEST
