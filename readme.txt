--[ about ]--------------------------------------------------------------------

This is clownfysh::cf, by Matthew Temple.  These are tools I use to build
things.  They're free; use them however you like.  For more, visit:
  + blog                                       ( http://clownfysh.tumblr.com/ )
  + twitter                                    ( http://twitter.com/clownfysh )
  + email                                               ( clownfysh@gmail.com )

--[ top-level systems ]--------------------------------------------------------

    bin :: environment, makefile, scripts
  chage :: chaos game engine
inferno :: search, classify, morph
  sorea :: os human-human, human-machine interface
 struct :: ...
   sync :: ...
      x :: objects, containers, networking

--[ setup ]--------------------------------------------------------------------

set up your environment
  + set $CF to the directory where you checked out this project (the directory
    containing this file)
  + add $CF/bin to $PATH (needed for the build)

install
  + OS development headers (stddef.h etc.), and set $CF_C_STANDARD_INCLUDE_DIR
    to the place on your OS that includes them
    (for example: /usr/lib/gcc/i486-linux-gnu/4.2.4/include)
  + an ANSI C compiler                  ( http://en.wikipedia.org/wiki/ANSI_C )
  + DirectFB                                       ( http://www.directfb.org/ )
  + IJG JPEG                                            ( http://www.ijg.org/ )
  + ncurses                            ( http://en.wikipedia.org/wiki/Ncurses )
  + OpenSSL                                         ( http://www.openssl.org/ )
  + POSIX Threads                ( http://en.wikipedia.org/wiki/POSIX_Threads )
  + PostgreSQL                                   ( http://www.postgresql.org/ )
    + compile with --enable-thread-safety
    + set $CF_POSTGRESQL_INCLUDE_DIR to the directory containing libpq-fe.h
      (for example: /usr/include/postgresql)
  + rsync                                ( http://en.wikipedia.org/wiki/Rsync )

develop and compile
  + "cd $CF"
  + "make help"
  + if you want to commit back to the project
    + read and follow the guide in doc/style.txt
    + "make test", "make release" and "make clean" before committing
