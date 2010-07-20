(setq auto-mode-alist
      (append '(
                ("\\.php$" . c-mode)
                ("\\.js$" . c-mode)
                )
              auto-mode-alist))

(setq-default abbrev-mode 't)
(setq-default dabbrev-abbrev-char-regexp nil)
(setq-default indent-tabs-mode nil)
(setq-default require-final-newline 't)
(setq-default show-trailing-whitespace 't)
(setq-default show-ws-hard-space 't)

(add-hook 'c-mode-common-hook 'trf-c-mode-common-hook)

(add-hook 'c-mode-hook
          '(lambda ()
             (make-local-hook 'find-file-hooks)
             (make-local-hook 'write-contents-hooks)
             ;;(add-hook 'find-file-hooks 'indent-buffer)
             (add-hook 'write-contents-hooks 'delete-trailing-whitespace)
             (add-hook 'write-contents-hooks 'untabify-buffer)
             ;;(add-hook 'write-contents-hooks 'indent-buffer)
             )
          )

(defun trf-c-mode-common-hook ()
  (c-set-style "bsd")
  (c-set-offset 'arglist-cont-nonempty '++)
  (c-set-offset 'case-label '+)
  (setq c-basic-offset 2)
  (setq tab-width 2)
  )

(defun indent-buffer()
  (interactive)
  ""
  (save-excursion
    (mark-whole-buffer)
    (indent-region (point-min) (point-max)))
  nil)

(defun untabify-buffer()
  (interactive)
  "Untabify the current buffer.  Return nil.  Must return nil, if added to write-file-hooks."
  (save-excursion
    (goto-char (point-min))
    (while (re-search-forward "[ \t]+$" nil t)
      (delete-region (match-beginning 0) (match-end 0)))
    (goto-char (point-min))
    (if (search-forward "\t" nil t)
        (untabify (1- (point)) (point-max))))
  nil)

(global-set-key "\C-b" 'electric-buffer-list)
(global-set-key "\C-f" 'find-file)
(global-set-key "\C-j" 'indent-region)
(global-set-key "\C-t" 'goto-line)
(global-set-key "\C-v" 'other-window)
(global-set-key "\M-#" 'tags-query-replace)
(global-set-key "\M-s" 'tags-search)

(setq inhibit-startup-message 1)
(menu-bar-mode 0)
(setq transient-mark-mode t)
(global-font-lock-mode 0)
(normal-erase-is-backspace-mode 0)
(setq trf-use-abbrevs 't)
(add-hook 'c-mode-common-hook
          (lambda () (c-toggle-auto-hungry-state 1)))

(put 'downcase-region 'disabled nil)
(put 'upcase-region 'disabled nil)

(keyboard-translate ?\C-h ?\C-?)

(define-abbrev-table 'c-mode-abbrev-table '(
    ("xarrayc" "trf_array_t *fz;
fz = trf_array_create(unsigned long initial_size,
        trf_object_compare_f compare_objects,
        trf_object_copy_f copy_object,
        trf_object_destroy_f destroy_object);
if (fz) {

} else {
    trace(\"trf_array_create\");
}" nil 0)
    ("xarrayd" "trf_array_destroy();" nil 0)
    ("xarrayi" "trf_array_iterate_start(fz);
while ((object = trf_array_iterate_next(fz))) {

}" nil 0)
    ("xas" "assert();" nil 0)
    ("xb" "bool_t" nil 0)
    ("xdo" "do {

} while ();" nil 0)
    ("xenum" "enum fz_t {

};
typedef enum fz_t fz_t;" nil 0)
    ("xf" "bool_false" nil 0)
    ("xfor" "for (; ; ) {

}" nil 0)
    ("xfunc" "_t f()
{
    assert();



    return ;
}" nil 0)
    ("xif" "if () {

}" nil 0)
    ("xife" "if () {

} else {

}" nil 0)
    ("xifep" "if () {

} else {
    trace(\"\");
}" nil 0)
    ("xinc" "#include \".h\"" nil 0)
    ("xle" "trf_audit_log_enter(log, \"\", \"\");" nil 0)
    ("xlistc" "trf_list_t *fz;
fz = trf_list_t *trf_list_create(trf_object_copy_f copy_object,
        trf_object_destroy_f destroy_object);
if (fz) {

} else {
    trace(\"trf_list_create\");
}" nil 0)
    ("xlistd" "trf_list_destroy();" nil 0)
    ("xlisti" "trf_list_iterate_start(fz);
while ((object = trf_list_iterate_next(fz))) {

}" nil 0)
    ("xlt" "trf_audit_log_trace(log, \"\", \"\");" nil 0)
    ("xmalloc" "fz = malloc(sizeof *fz);
if (fz) {

} else {
    trace(\"malloc\");
}" nil 0)
    ("xmxd" "if (pthread_mutex_destroy(&fz, NULL) != 0) {
    trace(\"pthread_mutex_destroy\");
}" nil 0)
    ("xmxi" "if (pthread_mutex_init(&fz, NULL) != 0) {
    trace(\"pthread_mutex_init\");
}" nil 0)
    ("xmx" "pthread_mutex_lock(&fz);
{
}
pthread_mutex_unlock(&fz);" nil 0)
    ("xo" "trf_object_t *" nil 0)
    ("xp" "printf(\"\", ...);" nil 0)
    ("xr" "return ;" nil 0)
    ("xsetc" "trf_set_t *fz;
fz = trf_set_create(trf_object_compare_f compare_objects,
        trf_object_copy_f copy_object,
        trf_object_destroy_f destroy_object);
if (fz) {

} else {
    trace(\"trf_set_create\");
}" nil 0)
    ("xsetd" "trf_set_destroy();" nil 0)
    ("xseti" "trf_set_iterate_start(fz);
while ((object = trf_set_iterate_next(fz))) {

}" nil 0)
    ("xstruct" "struct fz_t {

};
typedef struct fz_t fz_t;" nil 0)
    ("xt" "bool_true" nil 0)
    ("xtr" "trace(\"\");" nil 0)
    ("xw" "while () {

}" nil 0)
    ))

(define-abbrev-table 'fundamental-mode-abbrev-table '(
    ))

(define-abbrev-table 'global-abbrev-table '(
    ))

(define-abbrev-table 'lisp-mode-abbrev-table '(
    ))

(define-abbrev-table 'text-mode-abbrev-table '(
    ))
