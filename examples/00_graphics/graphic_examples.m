import py.codac4matlab.*

% Graphics can be directly called without a Figure2D instanciation, using "DefaultFigure":

DefaultFigure().set_window_properties(Vector({600,600}),Vector({300,300}));
DefaultFigure().draw_box(IntervalVector({{2.2,2.5},{2.2,2.5}}),StyleProperties({Color().black(),Color().yellow(0.5)}));
DefaultFigure().draw_AUV(Vector({1,1,3.14/2}),1.,StyleProperties({Color().black(),Color().yellow()},"-.."));
DefaultFigure().draw_motor_boat(Vector({0,0,0}),1.,StyleProperties({Color().black(),Color().yellow()}));
DefaultFigure().draw_tank(Vector({2,1,3.14/2}),1.,StyleProperties({Color().black(),Color().yellow()},"vehicles","-."));
DefaultFigure().draw_pie(Vector({2,2}),Interval(1.5,2.5),Interval((3*3.14/4)-0.5,(3*3.14/4)+0.5),StyleProperties({Color().blue(),Color().cyan()}));
DefaultFigure().draw_polyline({Vector({2,-0.5}),Vector({4,0.5}),Vector({3,1.5}),Vector({4,2.5}),Vector({3,3})},Color().red());
DefaultFigure().draw_polygon(Polygon({Vector({2,4.5}),Vector({4,4.5}),Vector({4.2,3.5}),Vector({3.5,3})}),StyleProperties({Color().none(),Color().green(0.5)}));
DefaultFigure().draw_polyline({Vector({-0.8,0}),Vector({0,1.5})},0.2,StyleProperties({Color().red(),Color().black(0.3)}));

% Last argument corresponds to "StyleProperties" with one or two colors: edge color + (optional) fill color
% Predefined Color objects can be configured with a float parameter for opacity (1=opaque, 0=transparent)

% Custom figures can also be created:

fig1 = Figure2D("My figure 1", GraphicOutput().VIBES.union(GraphicOutput().IPE));
% Here, graphics will be rendered by two tools: both VIBES and IPE
% For VIBES, it requires the VIBes viewer to be launched prior to the execution
% For IPE, it generates a file named "My figure 1.xml" that can be edited with IPE, and converted to PDF

fig1.set_window_properties(Vector({50,50}),Vector({500,500})); % position, window size
fig1.set_axes(axis(1,Interval(-10,10)), axis(2,Interval(-10,10))); % (axis_id,[range_of_values_on_this_axis])
fig1.draw_box(IntervalVector({{-1,1},{-1,1}}),StyleProperties({Color().green(),Color().red(0.2)})); % drawing a green box with red opacity values inside
fig1.draw_circle(Vector({1,1}),0.5,Color({255,155,5})); % drawing a circle at (1,1) of radius 0.5 with a custom RGB color
fig1.draw_ring(Vector({1,1}),Interval(4,6),Color().red()); % drawing a ring at (1,1) of radius [4,6] with a predefined red color

fig2 = Figure2D("My figure 2", GraphicOutput().VIBES.union(GraphicOutput().IPE));
fig2.set_axes(axis(1,Interval(-1,5)), axis(2,Interval(-1,5)));
fig2.set_window_properties(Vector({250,250}),Vector({500,500}));

% The previously declared figure "fig2" can now be used as a DefaultFigure
DefaultFigure().set(fig2);
DefaultFigure().draw_box(IntervalVector({{2.2,2.5},{2.2,2.5}}),StyleProperties({Color().black(),Color().green(0.8)}));

DefaultFigure().set(fig1);
DefaultFigure().draw_box(IntervalVector({{2.2,2.5},{2.2,2.5}}),StyleProperties({Color().blue(),Color().cyan(0.8)}));

fig2.draw_circle(Vector({2,2}),0.5,StyleProperties({Color().red(),Color().red(0.2)},"z:1.")); % will alway be on top despite being "drawn" first

fig2.draw_AUV(Vector({1,1,3.14/2}),2.,StyleProperties({Color().black(),Color().yellow()},"w:0.1","vehicles"));
fig2.draw_tank(Vector({2,1,3.14/2}),1.5,StyleProperties({Color().black(),Color().yellow()},"vehicles","w:0.1"));
fig2.draw_motor_boat(Vector({0,0,0}),1.,StyleProperties({Color().black(),Color().yellow()},"vehicles"));
fig2.draw_pie(Vector({2,2}),Interval(1.5,2.5),Interval((3*3.14/4)-0.5,(3*3.14/4)+0.5),StyleProperties({Color().blue(),Color().cyan()}));
fig2.draw_polyline({Vector({2,-0.5}),Vector({4,0.5}),Vector({3,1.5}),Vector({4,2.5}),Vector({3,3})},StyleProperties(Color().red(),".."));
fig2.draw_polygon(Polygon({Vector({2,4.5}),Vector({4,4.5}),Vector({4.2,3.5}),Vector({3.5,3})}),StyleProperties({Color().none(),Color().green(0.5)}));
fig2.draw_polyline({Vector({-0.8,0}),Vector({0,1.5})},0.2,StyleProperties({Color().red(),Color().black(0.3)}));
fig2.draw_ellipse(Vector({1,1}),Vector({0.5,2.}),0.2,StyleProperties({Color().blue(),Color().blue(0.3)}));
fig2.draw_line(Vector({1,1}),Vector({3,3}),Color().blue());
fig2.draw_arrow(Vector({3,1}),Vector({2.2,2}),0.2,StyleProperties({Color().red(),Color().black(0.3)}));
fig2.draw_parallelepiped(Parallelepiped(Vector({1.5,2.8}),Matrix({{0.5,0.4},{0,0.2}})),StyleProperties({Color().red(),Color().green(0.5)},"parallelepiped","w:0.1"));

fig2.draw_zonotope(Zonotope(Vector({4,1.5}),Matrix({{-0.2,-0.06,0.2,0.06,0.01,0.08,0}, ...
                                                    {0.1,0.04,0.04,-0.04,-0.03,0.18,0}})),StyleProperties({Color().red(),Color().yellow(0.4)},"zonotope","w:0.05"));

p_3d = Parallelepiped(Vector({1.2,3.5,2.2}),Matrix({{0.5,0.4,0},{0,0.2,0.1},{0,0,0.3}}));
fig2.draw_zonotope(p_3d.proj({1,2}),StyleProperties({Color().green(),Color().yellow(0.4)},"zonotope"));

fig2.draw_text("Hello, World!",Vector({-0.5,4.5}),0.2, Color().blue());

fig2.draw_raster("logo_codac.png", IntervalVector({{2.5,5},{-1,-0.4}}),StyleProperties("raster"));

fig2.draw_circle(Vector({0.5,0.5}),0.5,StyleProperties({Color().orange(),Color().orange(0.2)},"z:-1.")); % will alway be in the bottom despite being "drawn" last

% Colors
% predefined colors without and with opacity
fig2.draw_point(Vector({2,2}), StyleProperties({Color().red(),Color().yellow(0.5)}));
% HTML color without and with opacity
fig2.draw_box(IntervalVector({{2.4,2.9},{2.4,2.9}}),StyleProperties({Color("#da3907"),Color("#da390755")}));
% HSV color without and with opacity
fig2.draw_box(IntervalVector({{2.6,3.1},{2.6,3.1}}),StyleProperties({Color({108,90,78},Model().HSV),Color({108,90,78,20},Model().HSV)}));

fig3 = Figure2D("ColorMap figure", GraphicOutput().VIBES.union(GraphicOutput().IPE));
fig3.set_window_properties(Vector({800,250}),Vector({500,500}));

cmap_haxby = ColorMap().haxby();
cmap_default = ColorMap().basic();
cmap_blue_tube = ColorMap().blue_tube();
cmap_red_tube = ColorMap().red_tube();
cmap_rainbow = ColorMap().rainbow();

custom_map = ColorMap(Model().RGB);
custom_map.set_item(0,Color({255,0,0}));
custom_map.set_item(0.5,Color({0,255,0}));
custom_map.set_item(1,Color({0,0,255}));

subdivisions = 40;
fig3.set_axes(axis(1,Interval(-1,subdivisions+1)), axis(2,Interval(-1.25,0.05)));

for i = 0:(subdivisions)
    ratio = i / subdivisions;
    fig3.draw_box(IntervalVector({{i,i+1},{-1./5.,0}}),StyleProperties({Color().black(),cmap_default.color(ratio)}));
    fig3.draw_box(IntervalVector({{i,i+1},{-2./5.,-1./5.}}),StyleProperties({Color().black(),cmap_haxby.color(ratio)}));
    fig3.draw_box(IntervalVector({{i,i+1},{-3./5.,-2./5.}}),StyleProperties({Color().black(),cmap_rainbow.color(ratio)}));
    fig3.draw_box(IntervalVector({{i,i+1},{-4./5.,-3./5.}}),StyleProperties({Color().black(),cmap_blue_tube.color(ratio)}));
    fig3.draw_box(IntervalVector({{i,i+1},{-5./5.,-4./5.}}),StyleProperties({Color().black(),cmap_red_tube.color(ratio)}));
    fig3.draw_box(IntervalVector({{i,i+1},{-6./5.,-5./5.}}),StyleProperties({Color().black(),custom_map.color(ratio)}));
end

fig4 = Figure2D("My Figure 4", GraphicOutput().VIBES);
fig4.set_window_properties(Vector({500,50}),Vector({500,500}));
fig4.set_axes(axis(1,Interval(-10,10)), axis(2,Interval(-10,10)));

a= 0.8;
t = ScalarVar();
% Fermat's spiral
f1 = AnalyticFunction({t},vec(a*sqrt(t)*cos(t),a*sqrt(t)*sin(t)));
traj4 = AnalyticTraj(Interval(0,100),f1);
fig4.draw_trajectory(traj4, StyleGradientProperties(ColorMap().rainbow(), ".."));