(TeX-add-style-hook
 "main"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("acmart" "sigconf")))
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "url")
   (add-to-list 'LaTeX-verbatim-macros-with-braces-local "path")
   (add-to-list 'LaTeX-verbatim-macros-with-delims-local "url")
   (add-to-list 'LaTeX-verbatim-macros-with-delims-local "path")
   (TeX-run-style-hooks
    "latex2e"
    "body"
    "acmart"
    "acmart10"
    "booktabs"
    "url"
    "algpseudocode"
    "algorithm"
    "graphicx"
    "array"
    "multirow"
    "color"
    "float")
   (TeX-add-symbols
    '("rom" 1))
   (LaTeX-add-bibliographies
    "sample-bibliography"))
 :latex)

