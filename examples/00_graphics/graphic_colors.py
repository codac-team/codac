import math
from codac import *

def show_color(color_func, i):
  box = IntervalVector([math.floor(i / 3), i % 3]).inflate(0.4)
  DefaultFigure.draw_box(box, [color_func(1.0), color_func(1.0)])

i = 0

colors = [
  Color.light_gray, Color.gray, Color.dark_gray,
  Color.light_green, Color.green, Color.dark_green,
  Color.light_blue, Color.blue, Color.dark_blue,
  Color.light_cyan, Color.cyan, Color.dark_cyan,
  Color.light_yellow, Color.yellow, Color.dark_yellow,
  Color.light_orange, Color.orange, Color.dark_orange,
  Color.light_red, Color.red, Color.dark_red,
  Color.light_brown, Color.brown, Color.dark_brown,
  Color.light_purple, Color.purple, Color.dark_purple,
  Color.light_pink, Color.pink, Color.dark_pink,
]

for color_func in colors:
  show_color(color_func, i)
  i += 1

DefaultFigure.set_axes(
  axis(0, [-0.5, (i / 3) - 1 + 0.5]),
  axis(1, [-0.5, 2 + 0.5])
)

DefaultFigure.selected_fig().auto_scale()