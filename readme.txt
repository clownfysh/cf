C tools
  + bin                                      ( environment, makefile, scripts )
  + chage                                                 ( chaos game engine )
  + inferno                                         ( search, classify, morph )
  + x                                       ( objects, containers, networking )

Set up environment
  + point $CF to the directory containing this readme.txt file
  + add $CF/bin to $PATH

Install dependencies
  + C standard headers                                       ( stddef.h, etc. )
    + point $CF_C_STANDARD to them      ( somewhere like /usr/lib/gcc/include )
  + an ANSI C compiler                  ( http://en.wikipedia.org/wiki/ANSI_C )
  + POSIX Threads                ( http://en.wikipedia.org/wiki/POSIX_Threads )
  + ncurses                            ( http://en.wikipedia.org/wiki/Ncurses )
  + IJG JPEG                                            ( http://www.ijg.org/ )

Make
  + "cd $CF"
  + "make help" explains the make commands

If you want to commit back to the project
  + read and follow the guide in doc/style.txt
  + "make test", "make release" and "make clean" before committing

Matthew Temple
  + email                                               ( clownfysh@gmail.com )
  + twitter                                    ( http://twitter.com/clownfysh )
  + blog                                       ( http://clownfysh.tumblr.com/ )
  + portfolio                          ( http://cargocollective.com/clownfysh )
  + docs                                                 ( http://j.mp/91C1b8 )
  + code                                     ( http://github.com/clownfysh/cf )
