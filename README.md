# ft_printf
This is my own printf (language C), my first big language C project.

### Function prototype
```
int   ft_printf(const char *format, ...);
```

### Examples
```
ft_printf("%d\n", 42);
>> 42
```
```
ft_printf("%#10.*X\n", 25, 42);
>> 0X000000000000000000000002A
```


## Details
- Width managed
- Precision managed
- Wildcard managed

### Syntax
```
%[flags][width][.precision][length]type
```

### Conversions
| Character | Description |
|:-------------:|:-------------|
| dDi | int as a signed decimal number |
| c | Character |
| C | Unicode character |
| s | Null-terminated string |
| S | Unicode string |
| uU | Print decimal unsigned int |
| xX | Unsigned int as a hexadecimal number. x uses lower-case letters and X uses upper-case |
| p | Pointer to void |
| % | Print percentage |

### Flags field
| Flag | Description |
|:-------------:|:-------------|
| 0      | When the 'width' option is specified, prepends zeros for numeric types      |
| - | Left-align the output of this placeholder      |
| space | Prepends a space for positive signed-numeric types. positive =  , negative = -. This flag is ignored if the + flag exists.      |
