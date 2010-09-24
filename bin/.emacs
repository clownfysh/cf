(setq ih-use-abbrevs nil)
(if (file-exists-p "~/.emacs_cf") (load "~/.emacs_cf"))
(if (file-exists-p "~/.emacs_custom") (load "~/.emacs_custom"))
(if cf-use-abbrevs (load "~/.emacs_cf_abbrevs"))
