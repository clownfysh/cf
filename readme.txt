C tools by Matthew Temple
  + blog                                       ( http://clownfysh.tumblr.com/ )
  + twitter                                    ( http://twitter.com/clownfysh )
  + email                                               ( clownfysh@gmail.com )

What's here
  + bin :: environment, makefile, scripts
  + chage :: chaos game engine
  + inferno :: search, classify, morph
  + x :: objects, containers, networking

Set up environment
  + set $CF to the directory containing this file
  + add $CF/bin to $PATH

Install dependencies
  + development headers (stddef.h etc.)
    + set $CF_C_STANDARD to the place on your OS that includes them (for
      example: /usr/lib/gcc/i486-linux-gnu/4.2.4/include)
  + an ANSI C compiler                  ( http://en.wikipedia.org/wiki/ANSI_C )
  + POSIX Threads                ( http://en.wikipedia.org/wiki/POSIX_Threads )
  + ncurses                            ( http://en.wikipedia.org/wiki/Ncurses )
  + IJG JPEG                                            ( http://www.ijg.org/ )

Develop and compile
  + "cd $CF"
  + "make help"
  + if you want to commit back to the project
    + read and follow the guide in doc/style.txt
    + "make test", "make release" and "make clean" before committing
