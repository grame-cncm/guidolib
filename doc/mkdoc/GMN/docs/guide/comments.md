
## Comments and Spacing

White space, i.e, any combination of blank-, newline-, or tab-characters, can be inserted everywhere
except within [note](notes.md) or [tag-names](tags.md). The use of white space is discouraged in the following situations:

- within note specifications
- between tag-names and tag-argument-lists

White space is required in the following instances:
- between notes
- between tags
- between notes and tags and vice versa

### Comments

Comments can be inserted everywhere where white space is allowed. 
A comment started with `%` extends to the end of the line. 
Thus, lines beginning with '%' are comment-lines.

Alternately, `(* comment *)` can be used for specifying comments. 
This form of comments can include multiple lines and may be nested.

### Examples
~~~~~~sql
% a comment line
a0/4 h \slur(c1/8 h0) % a/4    <-- the last note is commented out

[ c1/4
(*                     comment starts here ...
d e f                  <-- part of the comment!
... ends here *)
d ]   % the sequence has only two notes!
~~~~~~

