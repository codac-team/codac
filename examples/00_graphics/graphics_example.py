from codac import *

# Graphics can be directly called without Figure instanciation, using "DefaultFigure":

DefaultFigure.set_window_properties([600,600],[300,300])
DefaultFigure.draw_box([[2.2,2.5],[2.2,2.5]],[Color.black(),Color.yellow(0.5)])
DefaultFigure.draw_AUV([1,1,3.14/2],1.,[Color.black(),Color.yellow()])
DefaultFigure.draw_tank([2,1,3.14/2],1.,[Color.black(),Color.yellow()])
DefaultFigure.draw_pie([2,2],[1.5,2.5],[(3*3.14/4)-0.5,(3*3.14/4)+0.5],[Color.blue(),Color.cyan()])
DefaultFigure.draw_polyline([[2,-0.5],[4,0.5],[3,1.5],[4,2.5],[3,3]], Color.red())
DefaultFigure.draw_polygone([[2,4.5],[4,4.5],[4.2,3.5],[3.5,3]], [Color.none(),Color.green(0.5)])
DefaultFigure.draw_polyline([[-0.8,0],[0,1.5]], 0.2, [Color.red(),Color.black(0.3)])

# Last argument corresponds to "StyleProperties" with two colors: edge color + (optional) fill color
# Predefined Color objects can be configured with a float parameter for opacity (1=opaque, 0=transparent)

# Custom figures can also be created:
fig1 = Figure("My figure 1", GraphicOutput.VIBES | GraphicOutput.IPE)
# Here, grapphics will be renderer with two tools: both VIBES and IPE
# For VIBES, it requires the VIBes viewer to be launch during prior to the execution
# For IPE, it generates a file names "My figure 1.xml" that can be edited with IPE, and converted to PDF

fig1.set_window_properties([50,50],[500,500]) # position, window size
fig1.set_axes(axis(0,[-10,10]), axis(1,[-10,10])) # (axis_id,[range_of_values_on_this_axis])
fig1.draw_box([[-1,1],[-1,1]],[Color.green(),Color.red(0.2)]) # drawing a green box with red opacity values inside
fig1.draw_circle([1,1],0.5,Color(255,155,5)) # drawing a circle at (1,1) of radius 0.5 with a custom RGB color
fig1.draw_ring([1,1],[4,6],Color.red()) # drawing a circle at (1,1) of radius 0.5 with a custom RGB color

fig2 = Figure("My figure 2", GraphicOutput.VIBES | GraphicOutput.IPE)
fig2.set_axes(axis(0,[-1,5]), axis(1,[-1,5]))
fig2.set_window_properties([250,250],[500,500])

# The previously declared figure "fig2" can now be used as a DefaultFigure
DefaultFigure.set(fig2)
DefaultFigure.draw_box([[2.2,2.5],[2.2,2.5]],[Color.black(),Color.green(0.8)])

DefaultFigure.set(fig1)
DefaultFigure.draw_box([[2.2,2.5],[2.2,2.5]],[Color.blue(),Color.cyan(0.8)])

fig2.draw_AUV([1,1,3.14/2],1.,[Color.black(),Color.yellow()])
fig2.draw_tank([2,1,3.14/2],1.,[Color.black(),Color.yellow()])
fig2.draw_pie([2,2],[1.5,2.5],[(3*3.14/4)-0.5,(3*3.14/4)+0.5],[Color.blue(),Color.cyan()])
fig2.draw_polyline([[2,-0.5],[4,0.5],[3,1.5],[4,2.5],[3,3]], Color.red())
fig2.draw_polygone([[2,4.5],[4,4.5],[4.2,3.5],[3.5,3]], [Color.none(),Color.green(0.5)])
fig2.draw_polyline([[-0.8,0],[0,1.5]], 0.2, [Color.red(),Color.black(0.3)])