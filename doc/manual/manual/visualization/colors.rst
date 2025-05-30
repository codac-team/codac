.. _sec-graphics-colors:

Styles, colors and color maps
=============================

  Main author: `Maël Godard <https://godardma.github.io>`_

Style
-----

By default, the drawn shapes will have a black edge and no fill. a StyleProperties object can be passed as an additionnal argument to change it.

Predefined styles are available in the StyleProperties class:

- inside() : black edge, green fill
- outside() : black edge, cyan fill
- boundary() : black edge, yellow fill

A StyleProperties object is composed of two Color objects, one for the edge and one for the fill. Three constructors are available:

.. tabs::

  .. code-tab:: py

    default_style = StyleProperties() # default
    edge_style = StyleProperties(Color.red()) # edge only
    edge_fill_style = StyleProperties([Color.blue(),Color.green()]) # edge and fill

  .. code-tab:: c++

    StyleProperties default_style;
    StyleProperties edge_style(Color::red()); // edge only
    StyleProperties edge_fill_style({Color::blue(),Color::green()}); // edge and fill


It can also be deduced from one or two Color objects.

.. tabs::

  .. code-tab:: py

    fig.draw_box([[2.2,2.5],[2.2,2.5]]) # Default style
    fig.draw_box([[2.2,2.5],[2.2,2.5]],StyleProperties.inside()) # black edge, green fill
    fig.draw_box([[2.2,2.5],[2.2,2.5]],Color.red()) # red edge, no fill
    fig.draw_box([[2.2,2.5],[2.2,2.5]],[Color.blue(),Color.green()]) # blue edge, green fill

  .. code-tab:: c++

    fig.draw_box({{2.2,2.5},{2.2,2.5}}); // Default style
    fig.draw_box({{2.2,2.5},{2.2,2.5}},StyleProperties::inside()); // black edge, green fill
    fig.draw_box({{2.2,2.5},{2.2,2.5}},Color::red()); // red edge, no fill
    fig.draw_box({{2.2,2.5},{2.2,2.5}},{Color::blue(),Color::green()}); // blue edge, green fill

In addition, a line style and/or a layer can be added to the StyleProperties object. The line style is defined by a string, and the layer is defined by its name (string).

Available line styles are:
  - "\-" (solid)
  - "\-\-" (dashed)
  - "\.\." (dotted)
  - "\-\." (dash-dotted)
  - "\-\.\." (dash-dot-dotted)

**These two arguments are optional, only one can be added and they can be added in any order.**

.. tabs::

  .. code-tab:: py
    
    fig.draw_box([[2.2,2.5],[2.2,2.5]], StyleProperties(Color.red(), "..", "layer1")) # Red edge, dotted line and layer1

  .. code-tab:: c++

    fig.draw_box({{2.2,2.5},{2.2,2.5}}, StyleProperties(Color.red(), "..", "layer1")); // Red edge, dotted line and layer1
    // fig.draw_box({{2.2,2.5},{2.2,2.5}}, {Color.red(), "..", "layer1"}); //equivalent

Colors
------

Predefined colors are available in the ``Color`` class. Each of the static methods can take an argument to define the transparency of the color between 0 (full transparency) and 1 (full opacity).

.. tabs::

  .. code-tab:: py

    Color.none(), Color.white(), Color.black()
    Color.light_gray(), Color.gray(), Color.dark_gray()
    Color.light_green(), Color.green(), Color.dark_green()
    Color.light_blue(), Color.blue(), Color.dark_blue()
    Color.light_cyan(), Color.cyan(), Color.dark_cyan()
    Color.light_yellow(), Color.yellow(), Color.dark_yellow()
    Color.light_orange(), Color.orange(), Color.dark_orange()
    Color.light_red(), Color.red(), Color.dark_red()
    Color.light_brown(), Color.brown(), Color.dark_brown()
    Color.light_purple(), Color.purple(), Color.dark_purple()
    Color.light_pink(), Color.pink(), Color.dark_pink()

  .. code-tab:: c++

    Color::none(), Color::white(), Color::black()
    Color::light_gray(), Color::gray(), Color::dark_gray()
    Color::light_green(), Color::green(), Color::dark_green()
    Color::light_blue(), Color::blue(), Color::dark_blue()
    Color::light_cyan(), Color::cyan(), Color::dark_cyan()
    Color::light_yellow(), Color::yellow(), Color::dark_yellow()
    Color::light_orange(), Color::orange(), Color::dark_orange()
    Color::light_red(), Color::red(), Color::dark_red()
    Color::light_brown(), Color::brown(), Color::dark_brown()
    Color::light_purple(), Color::purple(), Color::dark_purple()
    Color::light_pink(), Color::pink(), Color::dark_pink()

Each basic color is available in three shades: ``light_``, normal and ``dark_``:

.. figure:: img/codac_colors.png

Custom colors can be defined in the RGB or HSV color spaces. An enumaration Model is used to make the distinction between the two.

.. tabs::

  .. code-tab:: py
      
    Model.RGB # RGB color space
    Model.HSV # HSV color space

  .. code-tab:: c++

    Model::RGB; // RGB color space
    Model::HSV; // HSV color space

A getter model() is available, and the methods ``rgb()`` and ``hsv()`` are used to do the conversion between the two color spaces.

If the color is in RGB the red, green, blue and alpha values are between 0 and 255.
If the color is in HSV the hue value is between 0 and 360 while the saturation, value and alpha values are between 0 and 100.

The Color class constructor can take different arguments:

- No argument : black color
- An array of 3 floats and a Model (default is RGB): the RGB or HSV values
- An array of 4 floats and a Model (default is RGB): the RGBA or HSVA values and the transparency
- A list of 3 or 4 floats and a Model (default is RGB): the RGB, HSV, RGBA or HSVA values
- A string : the html representation of the color (e.g. "#FF0000" for red)

Additionnal methods are available for any useful purpose:

- ``hex_str()`` : the html representation of the color
- ``vec()`` : the RGBA or HSVA values in a vector

Color creation example :

.. tabs::

  .. code-tab:: py

    # predefined colors without and with opacity
    fig.draw_point([2,2], [Color.red(),Color.yellow(0.5)])
    # HTML color without and with opacity
    fig.draw_box([[2.4,2.9],[2.4,2.9]],[Color("#da3907"),Color("#da390755")])
    # HSV color without and with opacity
    fig.draw_box([[2.6,3.1],[2.6,3.1]],[Color([108,90,78],Model.HSV),Color([108,90,78,20],Model.HSV)])
    # RGB color auto cast from list without and with opacity
    fig.draw_box([[2.,2.3],[2.6,2.9]],[[255,0,255],[255,0,255,100]])

  .. code-tab:: c++

    // predefined colors without and with opacity
    fig.draw_point({2,2}, {Color::red(),Color::yellow(0.5)});
    // HTML color without and with opacity
    fig2.draw_box({{2.4,2.9},{2.4,2.9}},{Color("#da3907"),Color("#da390755")});
    // HSV color without and with opacity
    fig2.draw_box({{2.6,3.1},{2.6,3.1}},{Color({108,90,78},Model::HSV),Color({108,90,78,20},Model::HSV)});

Color maps
----------

Color maps are used to convert a scalar value (between 0 and 1) to a color. The ColorMap class provides a set of predefined color maps:

.. tabs::

  .. code-tab:: py

    ColorMap.basic() # default color map
    ColorMap.haxby() # Haxby color map
    ColorMap.rainbow() # rainbow color map
    ColorMap.blue_tube() # blue color map, used mainly for tubes
    ColorMap.red_tube() # red color map, used mainly for tubes

  .. code-tab:: c++

    ColorMap::basic(); // default color map
    ColorMap::haxby(); // Haxby color map
    ColorMap::rainbow(); // rainbow color map
    ColorMap::blue_tube(); // blue color map, used mainly for tubes
    ColorMap::red_tube(); // red color map, used mainly for tubes

These five color maps are displayed below:

.. figure:: img/colormaps.png

The method ``color()`` is used to get the color corresponding to a scalar value. The argument is a float between 0 and 1.

As for the Color class, the ColorMap also has a Model (RGB or HSV) and an associated getter model(). The default Model is RGB.

You can also create your own color map :

.. tabs::

  .. code-tab:: py

    # Create a custom color map
    custom_map = ColorMap(Model.RGB)
    custom_map[0] = Color([255,0,0])
    custom_map[0.5] = Color([0,255,0])
    custom_map[1] = Color([0,0,255])

  .. code-tab:: c++

    // Create a custom color map
    ColorMap custom_map (Model::RGB);
    custom_map[0] = Color({255,0,0});
    custom_map[0.5] = Color({0,255,0});
    custom_map[1] = Color({0,0,255});

Note that you can add RGB and HSV colors to the same color map. The model of the color map will define the interpolation space.