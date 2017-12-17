
static char	*GetString( char *record, char *buf, int i, int j );

static double 	GetDouble( char *record, int i, int j );

(defun c-parse-name ()
  (interactive)
  (skip-syntax-forward " ")
  (let ((start (point)))
    (search-chars-forward "^(")
    (buffer-substring start (point))
    (forward-char 2)))

(defun string-strip (str)
  (let ((start 1) (end 0) (current 1) ch)
    (while (and (> (length str) 0)
		(char-equal ?\- (char-syntax (string-to-char str))))
      (setq str (substring str 2))
      (setq start (1+ start)))
    (setq current start)
    (while (> (length str) 0)
      (if (and (char-equal ?\- (char-syntax (string-to-char str)))
	       last-normal-char)
	  (setq end current))
	  

(defun c-parse-parameters ()
  (interactive)

  ;; pre: cursor at "("

  (let ((pairs) variable type start)
    (while (not (char-equal ?\) (char-after (point))))
      (forward-char)
      (skip-syntax-forward " ")
      (setq start (point))
      (skip-syntax-forward "w_")
      (setq type (buffer-substring start (point)))
      (setq start (point))
      (skip-chars-forward "^,)")
      (setq variable (buffer-substring start (point)))
      (setq pairs (append pairs 
			  (mapcar 'string-strip (list (type variable))))))))
    

(defun c-parse-prototype (beg end)
  (interactive "r")
  (let (tokens)
    (goto-char beg)
    (setq tokens (list (c-parse-name)))

    (while (and (< (point) end)
		(setq tokens (append tokens (list (c-parse-parameters))))))

    tokens))

