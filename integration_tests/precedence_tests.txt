echo test1 && (echo a && echo b) || (echo d && echo k) && ls
(echo test2 || echo b && echo d) || (echo hello || echo bye)
(echo test3 && echo b) || (echo c && echo d)
exit
