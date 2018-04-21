set tabstop=4
set shiftwidth=4
set noexpandtab
set number
set showmatch
set nobackup
set nowritebackup
set splitright

nnoremap <f4> :make!<cr>
nnoremap <f5> :!./textadventure<cr>

:imap kj <Esc> 
:nmap <M-j> 5j
:nmap <M-k> 5k

let &t_ti.="\e[1 q"
let &t_SI.="\e[5 q"
let &t_EI.="\e[1 q"
let &t_te.="\e[0 q"
