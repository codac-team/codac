from codac import *

# Graphics can be directly called without a Figure2D instanciation, using "DefaultFigure":

DefaultFigure.set_window_properties([600,600],[300,300])
DefaultFigure.draw_box([[2.2,2.5],[2.2,2.5]],[Color.black(),Color.yellow(0.5)])
DefaultFigure.draw_AUV([1,1,3.14/2],1.,StyleProperties([Color.black(),Color.yellow()],"-.."))
DefaultFigure.draw_motor_boat([0,0,0], 1., [Color.black(),Color.yellow()])
DefaultFigure.draw_tank([2,1,3.14/2],1.,StyleProperties([Color.black(),Color.yellow()],"vehicles","-."))
DefaultFigure.draw_pie([2,2],[1.5,2.5],[(3*3.14/4)-0.5,(3*3.14/4)+0.5],[Color.blue(),Color.cyan()])
DefaultFigure.draw_polyline([[2,-0.5],[4,0.5],[3,1.5],[4,2.5],[3,3]], Color.red())
DefaultFigure.draw_polygon([[2,4.5],[4,4.5],[4.2,3.5],[3.5,3]], [Color.none(),Color.green(0.5)])
DefaultFigure.draw_polyline([[-0.8,0],[0,1.5]], 0.2, [Color.red(),Color.black(0.3)])

# Last argument corresponds to "StyleProperties" with one or two colors: edge color + (optional) fill color
# Predefined Color objects can be configured with a float parameter for opacity (1=opaque, 0=transparent)

# Custom figures can also be created:
fig1 = Figure2D("My figure 1", GraphicOutput.VIBES | GraphicOutput.IPE)
# Here, graphics will be rendered by two tools: both VIBES and IPE
# For VIBES, it requires the VIBes viewer to be launched prior to the execution
# For IPE, it generates a file named "My figure 1.xml" that can be edited with IPE, and converted to PDF

fig1.set_window_properties([50,50],[500,500]) # position, window size
fig1.set_axes(axis(0,[-10,10]), axis(1,[-10,10])) # (axis_id,[range_of_values_on_this_axis])
fig1.draw_box([[-1,1],[-1,1]],[Color.green(),Color.red(0.2)]) # drawing a green box with red opacity values inside
fig1.draw_circle([1,1],0.5,Color([255,155,5])) # drawing a circle at (1,1) of radius 0.5 with a custom RGB color
fig1.draw_ring([1,1],[4,6],Color.red()) # drawing a ring at (1,1) of radius [4,6] with a predefined red color

fig2 = Figure2D("My figure 2", GraphicOutput.VIBES | GraphicOutput.IPE)
fig2.set_axes(axis(0,[-1,5]), axis(1,[-1,5]))
fig2.set_window_properties([250,250],[500,500])

# The previously declared figure "fig2" can now be used as a DefaultFigure
DefaultFigure.set(fig2)
DefaultFigure.draw_box([[2.2,2.5],[2.2,2.5]],[Color.black(),Color.green(0.8)])

DefaultFigure.set(fig1)
DefaultFigure.draw_box([[2.2,2.5],[2.2,2.5]],[Color.blue(),Color.cyan(0.8)])

fig2.draw_AUV([1,1,3.14/2],2.,StyleProperties([Color.black(),Color.yellow()], "0.1", "vehicles"))
fig2.draw_tank([2,1,3.14/2],1.5,StyleProperties([Color.black(),Color.yellow()],"vehicles","0.1"))
fig2.draw_pie([2,2],[1.5,2.5],[(3*3.14/4)-0.5,(3*3.14/4)+0.5],[Color.blue(),Color.cyan()])
fig2.draw_polyline([[2,-0.5],[4,0.5],[3,1.5],[4,2.5],[3,3]], StyleProperties(Color.red(),".."))
fig2.draw_polygon([[2,4.5],[4,4.5],[4.2,3.5],[3.5,3]], [Color.none(),Color.green(0.5)])
fig2.draw_polyline([[-0.8,0],[0,1.5]], 0.2, [Color.red(),Color.black(0.3)])
fig2.draw_ellipse([1,1],[0.5,2], 0.2, [Color.blue(),Color.blue(0.3)])
fig2.draw_line([1,1],[3,3], Color.blue())
fig2.draw_arrow([3,1],[2.2,2], 0.2, [Color.red(),Color.black(0.3)])
fig2.draw_parallelepiped([1.5,2.8],Matrix([[0.5,0.4],[0,0.2]]), StyleProperties([Color.red(),Color.green(0.5)],"parallelepiped","0.1"))
fig2.draw_zonotope([4,1.5],[[-0.2,0.1],[-0.06,0.04],[0.2,0.04],
                 [0.06,-0.04],[0.01,-0.03],[0.08,0.18],[0,0]],
                StyleProperties([Color.red(),Color.yellow(0.4)],"zonotope","0.05"))

# Colors
# predefined colors without and with opacity
fig2.draw_point([2,2], [Color.red(),Color.yellow(0.5)])
# HTML color without and with opacity
fig2.draw_box([[2.4,2.9],[2.4,2.9]],[Color("#da3907"),Color("#da390755")])
# HSV color without and with opacity
fig2.draw_box([[2.6,3.1],[2.6,3.1]],[Color([108,90,78],Model.HSV),Color([108,90,78,20],Model.HSV)])
# RGB color auto cast from list without and with opacity
fig2.draw_box([[2.,2.3],[2.6,2.9]],[[255,0,255],[255,0,255,100]])

fig3 = Figure2D("ColorMap figure", GraphicOutput.VIBES | GraphicOutput.IPE)
fig3.set_window_properties([800,250],[500,500])

cmap_haxby=ColorMap.haxby()
cmap_default=ColorMap.basic()
cmap_blue_tube=ColorMap.blue_tube()
cmap_red_tube=ColorMap.red_tube()
cmap_rainbow=ColorMap.rainbow()

custom_map = ColorMap(Model.RGB)
custom_map[0] = Color([255,0,0])
custom_map[0.5] = Color([0,255,0])
custom_map[1] = Color([0,0,255])

subdivisions=40
fig3.set_axes(axis(0,[-1,subdivisions+1]), axis(1,[-1.25,0.05]))

for i in range (subdivisions):
    ratio=i/subdivisions
    fig3.draw_box([[i,i+1],[-1/5,0]],[Color.black(),cmap_default.color(ratio)])
    fig3.draw_box([[i,i+1],[-2/5,-1/5]],[Color.black(),cmap_haxby.color(ratio)])
    fig3.draw_box([[i,i+1],[-3/5,-2/5]],[Color.black(),cmap_rainbow.color(ratio)])
    fig3.draw_box([[i,i+1],[-4/5,-3/5]],[Color.black(),cmap_blue_tube.color(ratio)])
    fig3.draw_box([[i,i+1],[-5/5,-4/5]],[Color.black(),cmap_red_tube.color(ratio)])
    fig3.draw_box([[i,i+1],[-6/5,-5/5]],[Color.black(),custom_map.color(ratio)])

fig4 = Figure2D("My figure 4", GraphicOutput.VIBES)

fig4.set_window_properties([500,50],[500,500]) 
fig4.set_axes(axis(0,[-10,10]), axis(1,[-10,10])) 

a = 0.8
t=ScalarVar()
# Fermat's spiral
f1=AnalyticFunction([t], [a*sqrt(t)*cos(t),a*sqrt(t)*sin(t)])
traj4=AnalyticTraj(f1, [0,100])
fig4.draw_trajectory(traj4, ColorMap.rainbow())
