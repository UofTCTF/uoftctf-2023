# Flag

`uoft{TH30R3M_PR0V3D!}`

# How to obtain the flag

1. Deobfuscate the javascript (open-source tooling such as deobfuscate.relative.im works fine)
2. Derive the flag based on the checks.

Note: Challenge should not be run in internet explorer, only modern browsers or NodeJS.

## Deobfuscated and Documented Script:

```js
function checkPassword(_0x38d32a) {
  try {
    // Password length is 21.
    if (_0x38d32a.length !== 21) {
      return false;
    }
    if (
      _0x38d32a.slice(1, 2) !==
        (String.fromCodePoint + "")[
          parseInt((parseInt + "").charCodeAt(3), 16) - 147
        ] /* password[1] = 'p' */ ||
      _0x38d32a[(parseInt(41, 6) >> 2) - 2] !==
        String.fromCodePoint(123) /* password[4] = '{' */ ||
      _0x38d32a[4].charCodeAt(0) !==
        _0x38d32a[7].charCodeAt(0) + 72 /* password[7] = '3'. */ ||
      JSON.stringify(
        Array.from(
          _0x38d32a.slice(5, 7).split("").reverse().join(),
          (_0x2d4d73) => _0x2d4d73.codePointAt(0)
        ).map((_0x5b85c5) => _0x5b85c5 + 213)
      ) !==
        JSON.stringify([
          285, 257, 297,
        ]) /* password[5] = 'T', password[6] = 'H' */
    ) {
      return false;
    }
    /* For password[8], password[9], password[10], password[11] */
    let _0x3c7a5c = _0x38d32a.slice(8, 12).split("").reverse();

    try {
      for (let _0x396662 = 0; _0x396662 < 5; _0x396662++) {
        _0x3c7a5c[_0x396662] =
          _0x3c7a5c[_0x396662].charCodeAt(0) + _0x396662 + getAdder(_0x396662);
      }
    } catch (_0x1fbd51) {
      _0x3c7a5c = _0x3c7a5c.map(
        (_0x24cda7) => (_0x24cda7 += _0x1fbd51.constructor.name.length - 4)
      );
    }

    if (
      MD5(String.fromCodePoint(..._0x3c7a5c)) !==
      "098f6bcd4621d373cade4e832627b4f6" /* password[8] = '0', password[9] = 'R', password[10] = '3', password[11] = 'M' */
    ) {
      return false;
    }

    if (
      MD5(_0x38d32a.charCodeAt(12) + "") !==
      "812b4ba287f5ee0bc9d43bbf5bbe87fb" /* password[12] = '_' */
    ) {
      return false;
    }
    _0x3c7a5c = (_0x38d32a[8] + _0x38d32a[11]).split("");
    _0x3c7a5c.push(_0x3c7a5c.shift());
    if (
      _0x38d32a.substring(14, 16) !==
        String.fromCodePoint(
          ..._0x3c7a5c.map((_0x5b5ec8) =>
            Number.isNaN(+_0x5b5ec8) ? _0x5b5ec8.charCodeAt(0) + 5 : 48
          )
        ) /* password[14] = 'R' password[15] = '0' */ ||
      _0x38d32a[_0x38d32a[7] - _0x38d32a[10]] !==
        atob("dQ==") /* password[0] = 'u' */ ||
      _0x38d32a.indexOf(String.fromCharCode(117)) !==
        _0x38d32a[7] - _0x38d32a[17] /* password[17] = '3' */ ||
      JSON.stringify(
        _0x38d32a
          .slice(2, 4)
          .split("")
          .map(
            (_0x7bf0a6) =>
              _0x7bf0a6.charCodeAt(0) ^
              getAdder.name[_0x38d32a[7]].charCodeAt(0)
          )
      ) !==
        JSON.stringify(
          [72, 90].map(
            (_0x40ab0d) =>
              _0x40ab0d ^
              String.fromCodePoint.name[_0x38d32a[17] - 1].charCodeAt(0)
          )
        ) /* password[2] = 'f', password[3] = 't' */
    ) {
      return false;
    }
    if (
      String.fromCodePoint(
        ..._0x38d32a
          .split("")
          .filter(
            (_0x5edfac, _0x2965d2) => _0x2965d2 > 15 && _0x2965d2 % 2 == 0
          )
          .map(
            (_0x2ffa6d) =>
              _0x2ffa6d.charCodeAt(0) ^ (_0x38d32a.length + _0x38d32a[7])
          )
      ) !==
      atob(
        "g5Go"
      ) /* password[16] = 'V', password[18] = 'D', password[20] = '}' */
    ) {
      return false;
    }
    if (
      _0x38d32a[_0x38d32a.length - 2] !==
        String.fromCharCode(Math.floor((({} + "").charCodeAt(0) + 9) / 3)) ||
      _0x38d32a[1 + _0x38d32a[7]] !== giggity()[5] /* password[19] = ! */
    ) {
      return false;
    }
    return true;
  } catch (_0x4d4983) {
    return false;
  }
}
function getAdder(_0x430c9d) {
  switch (_0x430c9d) {
    case 0:
      return 34;
    case 1:
      return 44;
    case 2:
      return 26;
    case 3:
      return 60;
  }
  return 101;
}
function giggity() {
  return giggity.caller.name;
}
```
