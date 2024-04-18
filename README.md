## Serial connection parameters
| Baudrate   | Data Bits   | Parity Bit   | Stop Bits   |
|------------|-------------|--------------|-------------|
| 9600       | 8           | None         | 1           |

## Commands
| Command | Description | Response |
| --- | --- | --- |
| `#xxRD<CR>` | Read channel number xx from the monitor (must specify both hex digits; ie. for channel one use 01). Here <CR> is the carriage return character. The response contains the pressure in Torr using scientific notation. The first part of the response y.yy is the mantissa and zyy is the exponent, where z is the sign. | `*xx_y.yyEzyy<CR>` |
| `#xxSMy.yy<CR>` | Set the multiplier for channel xx used for converting from voltage to pressure in Torr. The multiplier is y.yy. The response indicates that the monitor has successfully updated the multiplier. | `*xx_PROGM_OK<CR>f` |

