window.MathJax = {
  jax: ["input/TeX", "input/MathML", "input/AsciiMath", "output/CommonHTML", "output/NativeMML"],
  extensions: ["tex2jax.js", "asciimath2jax.js", "mml2jax.js", "MathMenu.js", "MathZoom.js"],
  AsciiMath: {
    useMathMLspacing: true
  },
  TeX: {
    extensions: ["AMSmath.js", "AMSsymbols.js", "autoload-all.js"]
  },
  MathML: {
    extensions: ["mml3.js", "content-mathml.js"]
  },
  tex2jax: {
    inlineMath: [
      ['$', '$'],
      ["\\(", "\\)"]
    ],
    processEscapes: true
  },
  AuthorInit: function() {
    MathJax.Hub.Register.StartupHook("TeX AMSmath Ready", function() {
      var TEX = MathJax.InputJax.TeX;
      TEX.Definitions.Add({
        macros: {
shortleftarrow: ['Macro', '\\mathrel{\u2190}'],
shortrightarrow: ['Macro', '\\mathrel{\u2192}'],
shortuparrow: ['Macro', '\\mathrel{\u2191}'],
shortdownarrow: ['Macro', '\\mathrel{\u2193}'],
Yup: ['Macro', '\\mathbin{\u2144}'],
Ydown: ['Macro', '\\mathbin{}'],
Yleft: ['Macro', '\\mathbin{}'],
Yright: ['Macro', '\\mathbin{}'],
varcurlyvee: ['Macro', '\\mathbin{\u22CE}'],
varcurlywedge: ['Macro', '\\mathbin{\u22CF}'],
minuso: ['Macro', '\\mathbin{}'],
baro: ['Macro', '\\mathbin{}'],
sslash: ['Macro', '\\mathbin{\u2AFD}'],
bbslash: ['Macro', '\\mathbin{}'],
moo: ['Macro', '\\mathbin{}'],
varotimes: ['Macro', '\\mathbin{\u2297}'],
varoast: ['Macro', '\\mathbin{\u229B}'],
varobar: ['Macro', '\\mathbin{\u233D}'],
varodot: ['Macro', '\\mathbin{\u2299}'],
varoslash: ['Macro', '\\mathbin{\u2298}'],
varobslash: ['Macro', '\\mathbin{\u29B8}'],
varocircle: ['Macro', '\\mathbin{\u229A}'],
varoplus: ['Macro', '\\mathbin{\u2295}'],
varominus: ['Macro', '\\mathbin{\u2296}'],
boxast: ['Macro', '\\mathbin{\u29C6}'],
boxbar: ['Macro', '\\mathbin{\u25EB}'],
boxdot: ['Macro', '\\mathbin{\u22A1}'],
boxslash: ['Macro', '\\mathbin{\u29C4}'],
boxbslash: ['Macro', '\\mathbin{\u29C5}'],
boxcircle: ['Macro', '\\mathbin{\u29C7}'],
boxbox: ['Macro', '\\mathbin{\u29C8}'],
boxempty: ['Macro', '\\mathbin{}'],
lightning: ['Macro', '\\mathbin{}'],
merge: ['Macro', '\\mathbin{}'],
vartimes: ['Macro', '\\mathbin{\u00D7}'],
fatsemi: ['Macro', '\\mathbin{}'],
sswarrow: ['Macro', '\\mathrel{}'],
ssearrow: ['Macro', '\\mathrel{}'],
curlywedgeuparrow: ['Macro', '\\mathrel{}'],
curlywedgedownarrow: ['Macro', '\\mathrel{}'],
fatslash: ['Macro', '\\mathbin{}'],
fatbslash: ['Macro', '\\mathbin{}'],
lbag: ['Macro', '\\mathbin{\u27C5}'],
rbag: ['Macro', '\\mathbin{\u27C6}'],
varbigcirc: ['Macro', '\\mathbin{\u25CB}'],
leftrightarroweq: ['Macro', '\\mathrel{}'],
curlyveedownarrow: ['Macro', '\\mathrel{}'],
curlyveeuparrow: ['Macro', '\\mathrel{}'],
nnwarrow: ['Macro', '\\mathrel{}'],
nnearrow: ['Macro', '\\mathrel{}'],
leftslice: ['Macro', '\\mathbin{}'],
rightslice: ['Macro', '\\mathbin{}'],
varolessthan: ['Macro', '\\mathbin{\u29C0}'],
varogreaterthan: ['Macro', '\\mathbin{\u29C1}'],
varovee: ['Macro', '\\mathbin{}'],
varowedge: ['Macro', '\\mathbin{}'],
talloblong: ['Macro', '\\mathbin{\u2AFE}'],
interleave: ['Macro', '\\mathbin{\u2AF4}'],
circledast: ['Macro', '"229B}'],
ocircle: ['Macro', '"229A}'],
circledcirc: ['Macro', '"229A}'],
obar: ['Macro', '\\mathbin{\u233D}'],
obslash: ['Macro', '\\mathbin{\u29B8}'],
olessthan: ['Macro', '\\mathbin{\u29C0}'],
ogreaterthan: ['Macro', '\\mathbin{\u29C1}'],
ovee: ['Macro', '\\mathbin{}'],
owedge: ['Macro', '\\mathbin{}'],
oblong: ['Macro', '\\mathbin{}'],
inplus: ['Macro', '\\mathrel{}'],
niplus: ['Macro', '\\mathrel{}'],
nplus: ['Macro', '\\mathbin{}'],
subsetplus: ['Macro', '\\mathrel{\u2ABF}'],
supsetplus: ['Macro', '\\mathrel{\u2AC0}'],
subsetpluseq: ['Macro', '\\mathrel{}'],
supsetpluseq: ['Macro', '\\mathrel{}'],
Lbag: ['Macro', '\\mathopen{}'],
Rbag: ['Macro', '\\mathclose{}'],
llparenthesis: ['Macro', '\\mathopen{\u2987}'],
rrparenthesis: ['Macro', '\\mathclose{\u2988}'],
binampersand: ['Macro', '\\mathopen{}'],
bindnasrepma: ['Macro', '\\mathclose{}'],
trianglelefteqslant: ['Macro', '\\mathrel{}'],
trianglerighteqslant: ['Macro', '\\mathrel{}'],
ntrianglelefteqslant: ['Macro', '\\mathrel{}'],
ntrianglerighteqslant: ['Macro', '\\mathrel{}'],
llfloor: ['Macro', '\\mathopen{}'],
rrfloor: ['Macro', '\\mathclose{}'],
llceil: ['Macro', '\\mathopen{}'],
rrceil: ['Macro', '\\mathclose{}'],
Mapstochar: ['Macro', '\\mathrel{}'],
mapsfromchar: ['Macro', '\\mathrel{}'],
Mapsfromchar: ['Macro', '\\mathrel{}'],
leftrightarrowtriangle: ['Macro', '\\mathrel{\u21FF}'],
leftarrowtriangle: ['Macro', '\\mathrel{\u21FD}'],
rightarrowtriangle: ['Macro', '\\mathrel{\u21FE}'],
bigtriangledown: ['Macro', '\\mathop {\u25BD}'],
bigtriangleup: ['Macro', '\\mathop {\u25B3}'],
bigcurlyvee: ['Macro', '\\mathop{}'],
bigcurlywedge: ['Macro', '\\mathop{}'],
bigsqcap: ['Macro', '\\mathop {\u2A05}'],
bigbox: ['Macro', '\\mathop{}'],
bigparallel: ['Macro', '\\mathop{}'],
biginterleave: ['Macro', '\\mathop {\u2AFC}'],
bignplus: ['Macro', '\\mathop{}'],
llbracket: ['Macro', '\\mathopen{\u27E6}'],
rrbracket: ['Macro', '\\mathclose{\u27E7}'],

// I think this is a copyright sign with a circular rather than slightly elliptical perimeter.

varcopyright: ['Macro', '{\u00A9}'],

// The following four macros are like \not, but adjusted for use with arrows.

arrownot: ['Macro', '\\mathrel{}'],
Arrownot: ['Macro', '\\mathrel{}'],
longarrownot: ['Macro', '\\mathrel{}'],
Longarrownot: ['Macro', '\\mathrel{}'],

Mapsto: ['Macro', '\\mathrel{\u21A6}'],
mapsfrom: ['Macro', '\\mathrel{\u21A4}'],
Mapsfrom: ['Macro', '\\mathrel{\u2906}'],
Longmapsto: ['Macro', '\\mathrel{\u27FE}'],
longmapsfrom: ['Macro', '\\mathrel{\u27FB}'],
Longmapsfrom: ['Macro', '\\mathrel{\u27FD}']

          // % \ifstmry@heavy@}
          // %    \def\@swap#1#2{\let\@tempa#1\let#1#2\let#2\@tempa}}
          // %    \@swap\varotimes\otimes}
          // %    \@swap\varolessthan\olessthan}
          // %    \@swap\varogreaterthan\ogreaterthan}
          // %    \@swap\varovee\ovee}
          // %    \@swap\varowedge\owedge}
          // %    \@swap\varoast\oast}
          // %    \@swap\varobar\obar}
          // %    \@swap\varodot\odot}
          // %    \@swap\varoslash\oslash}
          // %    \@swap\varobslash\obslash}
          // %    \@swap\varocircle\ocircle}
          // %    \@swap\varoplus\oplus}
          // %    \@swap\varominus\ominus}
          // %    \@swap\varbigcirc\bigcirc}
          // %    \@swap\varcopyright\copyright}
          // % \fi}

        }
      });
    });
  }
};

(function(d, script) {
  script = d.createElement('script');
  script.type = 'text/javascript';
  script.async = true;
  script.onload = function() {
    // remote script has loaded
  };
  script.src = 'https://cdn.mathjax.org/mathjax/latest/MathJax.js';
  d.getElementsByTagName('head')[0].appendChild(script);
}(document));