// This file is part of VIBes' C++ API
//
// Copyright (c) 2013-2015 Vincent Drevelle, Jeremy Nicola, Simon Rohou,
//                         Benoit Desrochers, Maël Godard
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "vibes.h"
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <limits>
#include <iomanip>
#include <memory>

//
// Vibes properties key,value system implementation
//


namespace vibes {
    std::string Value::toJSONString() const {
        std::ostringstream ss;
        switch (_type) {
        case vt_integer:
            ss<<_integer; break;
        case vt_decimal:
            ss<< std::setprecision(std::numeric_limits<double>::digits10) <<_decimal; break;
        case vt_string:
            ss<<'"'<<_string<<'"'; break;
        case vt_array:
            ss << '[';
            for (std::vector<Value>::const_iterator it = _array.begin(); it != _array.end(); ++it) {
                if (it != _array.begin()) ss << ',';
                ss << it->toJSONString();
            }
            ss << ']';
            break;
        case vt_object:
            ss << '{' << _object->toJSON() << '}';
            break;
        case vt_none:
        default:
            break;
        }
        return ss.str();
    }

    std::string Params::toJSON() const {
        std::ostringstream ss;
        for(std::map<std::string, Value>::const_iterator it = _values.begin(); it != _values.end(); ++it)
            ss << (it==_values.begin()?"":", ") << "\"" << it->first << "\":" << it->second.toJSONString();
        return ss.str();
    }

    Value Params::pop(const std::string &key, const Value &value_not_found) {
        KeyValueMap::iterator it = _values.find(key);
        // Return empty value if not found
        if (it == _values.end())
            return value_not_found;
        // Otherwise, return corresponding value and remove it from map
        Value val = it->second;
        _values.erase(it);
        return val;
    }
}

//
// Vibes messaging implementation
//

using namespace std;

namespace vibes
{
  //
  // Global variables and utility functions
  //
  namespace {

      /// Current communication file descriptor
      // FILE *channel=nullptr;
      std::shared_ptr<FILE> channel;

      /// Current figure name (client-maintained state)
      string current_fig="default";

  }

  //
  // Management of connection to the Vibes server
  //

  void beginDrawing()
  {
      // Retrieve user-profile directory from envirnment variable
      char * user_dir = getenv("USERPROFILE"); // Windows
      if (!user_dir)
          user_dir = getenv("HOME"); // POSIX
      if (user_dir)
      { // Environment variable found, connect to a file in user's profile directory
          std::string file_name(user_dir);
          file_name.append("/.vibes.json");
          beginDrawing(file_name);
      }
      else
      { // Connect to a file in working directory
          beginDrawing("vibes.json");
      }
  }

  void beginDrawing(const std::string &fileName)
  {
    if (!channel)
      channel = std::shared_ptr<FILE>(
        fopen(fileName.c_str(),"a"),
        // Callback for automatically closing the file
        // when the shared_ptr is released:
        [](FILE* file) {
          if(file)
            fclose(file);
        }
      );
  }

  void beginDrawingIfNeeded()
  {
    if (!channel)
    {
      beginDrawing();
    }
  }

  void endDrawing()
  {

  }



  //
  // Figure management
  //

  void newFigure(const std::string &figureName)
  {
    beginDrawingIfNeeded();
    std::string msg;
    if (!figureName.empty()) current_fig = figureName;
    msg ="{\"action\":\"new\","
          "\"figure\":\""+(figureName.empty()?current_fig:figureName)+"\"}\n\n";
    fputs(msg.c_str(),channel.get());
    fflush(channel.get());
  }

  void clearFigure(const std::string &figureName)
  {
    beginDrawingIfNeeded();
    std::string msg;
    msg="{\"action\":\"clear\","
         "\"figure\":\""+(figureName.empty()?current_fig:figureName)+"\"}\n\n";
    fputs(msg.c_str(),channel.get());
    fflush(channel.get());
  }

  void closeFigure(const std::string &figureName)
  {
    beginDrawingIfNeeded();
    std::string msg;
    msg="{\"action\":\"close\","
         "\"figure\":\""+(figureName.empty()?current_fig:figureName)+"\"}\n\n";
    fputs(msg.c_str(),channel.get());
    fflush(channel.get());
  }

  void saveImage(const std::string &fileName, const std::string &figureName)
  {
    beginDrawingIfNeeded();
    std::string msg;
    msg="{\"action\":\"export\","
          "\"figure\":\""+(figureName.empty()?current_fig:figureName)+"\","
          "\"file\":\""+fileName+"\"}\n\n";
    fputs(msg.c_str(),channel.get());
    fflush(channel.get());
  }

  void selectFigure(const std::string &figureName)
  {
    beginDrawingIfNeeded();
    current_fig = figureName;
  }


  //
  // View settings
  //

  void axisAuto(const std::string &figureName)
  {
    beginDrawingIfNeeded();
    setFigureProperty(figureName.empty()?current_fig:figureName, "viewbox", "auto");
  }

  //>[#144]
  void axisEqual(const std::string &figureName)
  {
    beginDrawingIfNeeded();
    setFigureProperty(figureName.empty()?current_fig:figureName, "viewbox", "equal");
  }
  //<[#144]

  void axisLimits(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub, const std::string &figureName)
  {
    beginDrawingIfNeeded();
  Vec4d v4d = { x_lb, x_ub, y_lb, y_ub };
    setFigureProperty(figureName.empty()?current_fig:figureName, "viewbox", v4d);
  }

  void axisLabels(const std::string &x_label, const std::string &y_label, const std::string &figureName)
  {
    beginDrawingIfNeeded();
    vector<string> labels;
    labels.push_back(x_label);
    labels.push_back(y_label);
    axisLabels(labels, figureName);
  }

  void axisLabels(const std::vector<std::string> &labels, const std::string &figureName)
  {
    beginDrawingIfNeeded();
    setFigureProperty( figureName.empty()?current_fig:figureName, "axislabels", labels);
  }


  //
  // Drawing functions
  //

  void drawBox(const double &x_lb, const double &x_ub, const double &y_lb, const double &y_ub, Params params)
  {
    beginDrawingIfNeeded();
  Vec4d v4d = { x_lb, x_ub, y_lb, y_ub };
    Params msg;
    msg["action"] = "draw";
    msg["figure"] = params.pop("figure",current_fig);
    msg["shape"] = (params, "type", "box", "bounds", v4d);

    fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
    fflush(channel.get());
  }

  void drawBox(const vector<double> &bounds, Params params)
  {
    beginDrawingIfNeeded();
    assert(!bounds.empty());
    assert(bounds.size()%2 == 0);

    Params msg;
    msg["action"] = "draw";
    msg["figure"] = params.pop("figure",current_fig);
    msg["shape"] = (params, "type", "box", "bounds", vector<Value>(bounds.begin(),bounds.end()));

    fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
    fflush(channel.get());
  }


  void drawEllipse(const double &cx, const double &cy, const double &a, const double &b, const double &rot, Params params)
  {
    beginDrawingIfNeeded();
    Vec2d vc = { cx, cy };
    Vec2d va = {a, b};
      Params msg;
      msg["action"] = "draw";
      msg["figure"] = params.pop("figure",current_fig);
      msg["shape"] = (params, "type", "ellipse",
                              "center", vc,
                              "axis", va,
                              "orientation", rot);

      fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
      fflush(channel.get());
  }

  void drawConfidenceEllipse(const double &cx, const double &cy,
                             const double &sxx, const double &sxy, const double &syy,
                             const double &K, Params params)
  {
    beginDrawingIfNeeded();
    Vec2d vc = {cx, cy};
    Vec4d vcov = { sxx, sxy, sxy, syy };
      Params msg;
      msg["action"] = "draw";
      msg["figure"] = params.pop("figure",current_fig);
      msg["shape"] = (params, "type", "ellipse",
                              "center", vc,
                              "covariance", vcov,
                              "sigma", K);

      fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
      fflush(channel.get());
  }

  void drawConfidenceEllipse(const vector<double> &center, const vector<double> &cov,
                             const double &K, Params params)
  {
      beginDrawingIfNeeded();
      Params msg;
      msg["action"] = "draw";
      msg["figure"] = params.pop("figure",current_fig);
      msg["shape"] = (params, "type", "ellipse",
                              "center", vector<Value>(center.begin(),center.end()),
                              "covariance", vector<Value>(cov.begin(),cov.end()),
                              "sigma", K);

      fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
      fflush(channel.get());
  }

  void drawSector(const double &cx, const double &cy, const double &a, const double &b,
                  const double &startAngle, const double &endAngle, Params params)
  {
      beginDrawingIfNeeded();
      // Angle need to be in degree
      Params msg;
      Vec2d cxy={ cx, cy };
      Vec2d cab = { a, b };
      Vec2d startEnd = { startAngle, endAngle };
      msg["action"] = "draw";
      msg["figure"] = params.pop("figure",current_fig);
      msg["shape"] = (params, "type", "ellipse",
                              "center", cxy,
                              "axis", cab,
                              "orientation", 0,
                              "angles", startEnd);

      fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
      fflush(channel.get());
  }

  void drawPie(const double &cx, const double &cy, const double &r_min, const double &r_max,
                  const double &theta_min, const double &theta_max, Params params)
  {
      beginDrawingIfNeeded();
      // Angle need to be in degree
      Params msg;
      Vec2d cxy = { cx, cy };
      Vec2d rMinMax = { r_min, r_max };
      Vec2d thetaMinMax = { theta_min, theta_max };
      msg["action"] = "draw";
      msg["figure"] = params.pop("figure",current_fig);
      msg["shape"] = (params, "type", "pie",
                              "center", cxy,
                              "rho", rMinMax,
                              "theta", thetaMinMax);

      fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
      fflush(channel.get());
  }

  void drawPoint(const double &cx, const double &cy, Params params)
  {
      beginDrawingIfNeeded();
      Params msg;
      Vec2d cxy = { cx, cy };
      msg["action"]="draw";
      msg["figure"]=params.pop("figure",current_fig);
      msg["shape"]=(params, "type","point",
                            "point",cxy);
      fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
      fflush(channel.get());
  }

  void drawPoint(const double &cx, const double &cy, const double &radius, Params params)
  {
      beginDrawingIfNeeded();
      Params msg;
      Vec2d cxy = { cx, cy };
      msg["action"]="draw";
      msg["figure"]=params.pop("figure",current_fig);
      msg["shape"]=(params, "type","point",
                            "point",cxy,"Radius",radius);
      fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
      fflush(channel.get());
  }

  void drawRing(const double &cx, const double &cy, const double &r_min, const double &r_max, Params params)
  {
      beginDrawingIfNeeded();
      Params msg;
      Vec2d cxy = { cx, cy };
      Vec2d rMinMax = { r_min, r_max };
      msg["action"] = "draw";
      msg["figure"] = params.pop("figure",current_fig);
      msg["shape"] = (params, "type", "ring",
                              "center", cxy,
                              "rho", rMinMax);
      fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
      fflush(channel.get());
  }

  void drawBoxes(const std::vector<std::vector<double> > &bounds, Params params)
  {
     beginDrawingIfNeeded();
     Params msg;
     msg["action"] = "draw";
     msg["figure"] = params.pop("figure",current_fig);
     msg["shape"] = (params, "type", "boxes",
                             "bounds", bounds);

     fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
     fflush(channel.get());
  }

  void drawBoxesUnion(const std::vector<std::vector<double> > &bounds, Params params)
  {
     beginDrawingIfNeeded();
     Params msg;
     msg["action"] = "draw";
     msg["figure"] = params.pop("figure",current_fig);
     msg["shape"] = (params, "type", "boxes union",
                             "bounds", bounds);

     fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
     fflush(channel.get());
  }

  void drawLine(const std::vector<std::vector<double> > &points, Params params)
  {
     beginDrawingIfNeeded();
     Params msg;
     msg["action"] = "draw";
     msg["figure"] = params.pop("figure",current_fig);
     msg["shape"] = (params, "type", "line",
                             "points", points);

     fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
     fflush(channel.get());
  }

  void drawLine(const std::vector<double> &x, const std::vector<double> &y, Params params)
  {
     beginDrawingIfNeeded();
     // Reshape x and y into a vector of points
     std::vector<Value> points;
     std::vector<double>::const_iterator itx = x.begin();
     std::vector<double>::const_iterator ity = y.begin();
   Vec2d vp;
     while (itx != x.end() && ity != y.end()) {
    vp._data[0] = *itx++;
    vp._data[1] = *ity++;
        points.push_back( vp );
     }
     // Send message
     Params msg;
     msg["action"] = "draw";
     msg["figure"] = params.pop("figure",current_fig);
     msg["shape"] = (params, "type", "line",
                             "points", points);

     fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
     fflush(channel.get());
  }

  //void drawPoints(const std::vector<std::vector<double> > &points, Params params)
  //{
  //    Params msg;
  //    msg["action"]="draws";
  //    msg["figure"] = params.pop("figure",current_fig);
  //    msg["shape"] = (params, "type", "points",
  //                           "points", points);
  //    fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
  //    fflush(channel.get());
  //}

  //void drawPoints(const std::vector<std::vector<double> > &points,  const std::vector<double> &colorLevels, const std::vector<double> &radiuses, Params params)
  //{
  //    Params msg;
  //    msg["action"]="draws";
  //    msg["figure"] = params.pop("figure",current_fig);
  //    msg["shape"] = (params, "type", "points",
  //                           "points", points,
  //                           "colorLevels", colorLevels,
  //                           "radiuses", radiuses);
  //    fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
  //    fflush(channel.get());
  //}

  void drawPoints(const std::vector<double> &x, const std::vector<double> &y, Params params)
  {
     beginDrawingIfNeeded();
      // Reshape x and y into a vector of points
     std::vector<Value> points;
     std::vector<double>::const_iterator itx = x.begin();
     std::vector<double>::const_iterator ity = y.begin();
   Vec2d vp;
     while (itx != x.end() && ity != y.end()) {
    vp._data[0] = *itx++;
    vp._data[1] = *ity++;
        points.push_back( vp );
     }
     // Send message
     Params msg;
     msg["action"] = "draw";
     msg["figure"] = params.pop("figure",current_fig);
     msg["shape"] = (params, "type", "points",
                             "centers", points);

     fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
     fflush(channel.get());
  }

  //void drawPoints(const std::vector<double> &x, const std::vector<double> y, const std::vector<double> &colorLevels, Params params)
  //{
  //    // Reshape x and y into a vector of points
  //   std::vector<Value> points;
  //   std::vector<double>::const_iterator itx = x.begin();
  //   std::vector<double>::const_iterator ity = y.begin();
//   Vec2d vp;
  //   while (itx != x.end() && ity != y.end()) {
//    vp._data[0] = *itx++;
//    vp._data[1] = *ity++;
  //      points.push_back( vp );
  //   }
  //   // Send message
  //   Params msg;
  //   msg["action"] = "draw";
  //   msg["figure"] = params.pop("figure",current_fig);
  //   msg["shape"] = (params, "type", "points",
  //                           "points", points,
  //                           "colorLevels", colorLevels);
//
  //   fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
  //   fflush(channel.get());
  //}

  //void drawPoints(const std::vector<double> &x, const std::vector<double> y, const std::vector<double> &colorLevels, const std::vector<double> &radiuses, Params params)
  //{
  //   // Reshape x and y into a vector of points
  //   std::vector<Value> points;
  //   std::vector<double>::const_iterator itx = x.begin();
  //   std::vector<double>::const_iterator ity = y.begin();
//   Vec2d vp;
  //   while (itx != x.end() && ity != y.end()) {
//    vp._data[0] = *itx++;
//    vp._data[1] = *ity++;
  //      points.push_back( vp );
  //   }
  //   // Send message
  //   Params msg;
  //   msg["action"] = "draw";
  //   msg["figure"] = params.pop("figure",current_fig);
  //   msg["shape"] = (params, "type", "points",
  //                           "points", points,
  //                           "colorLevels", colorLevels,
  //                           "radiuses", radiuses);
//
  //   fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
  //   fflush(channel.get());
  //}

  void drawArrow(const double &xA, const double &yA, const double &xB, const double &yB, const double &tip_length, Params params)
  {
    beginDrawingIfNeeded();
    // Reshape A and B into a vector of points
    std::vector<Value> points;
    Vec2d va = { xA, yA };
    Vec2d vb = { xB, yB };
    points.push_back(va);
    points.push_back(vb);

    // Send message
    Params msg;
    msg["action"] = "draw";
    msg["figure"] = params.pop("figure",current_fig);
    msg["shape"] = (params, "type", "arrow",
                           "points", points,
                           "tip_length", tip_length);

    fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
    fflush(channel.get());
  }

  void drawArrow(const std::vector<std::vector<double> > &points, const double &tip_length, Params params)
  {
    beginDrawingIfNeeded();
    Params msg;
    msg["action"] = "draw";
    msg["figure"] = params.pop("figure",current_fig);
    msg["shape"] = (params, "type", "arrow",
                           "points", points,
                           "tip_length", tip_length);

    fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
    fflush(channel.get());
  }

  void drawArrow(const std::vector<double> &x, const std::vector<double> &y, const double &tip_length, Params params)
  {
    beginDrawingIfNeeded();
    // Reshape x and y into a vector of points
    std::vector<Value> points;
    std::vector<double>::const_iterator itx = x.begin();
    std::vector<double>::const_iterator ity = y.begin();
    Vec2d vp;
    while (itx != x.end() && ity != y.end()) {
    vp._data[0] = *itx++;
    vp._data[1] = *ity++;
        points.push_back( vp );
    }
    // Send message
    Params msg;
    msg["action"] = "draw";
    msg["figure"] = params.pop("figure",current_fig);
    msg["shape"] = (params, "type", "arrow",
                            "points", points,
                            "tip_length", tip_length);

    fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
    fflush(channel.get());
  }

  void drawPolygon(const std::vector<double> &x, const std::vector<double> &y, Params params)
  {
    beginDrawingIfNeeded();
    // Reshape x and y into a vector of points
    std::vector<Value> points;
    std::vector<double>::const_iterator itx = x.begin();
    std::vector<double>::const_iterator ity = y.begin();
    Vec2d vp;
    while (itx != x.end() && ity != y.end()) {
      vp._data[0] = *itx++;
      vp._data[1] = *ity++;
      points.push_back( vp );
    }
    // Send message
    Params msg;
    msg["action"] = "draw";
    msg["figure"] = params.pop("figure",current_fig);
    msg["shape"] = (params, "type", "polygon",
                           "bounds", points);

    fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
    fflush(channel.get());
  }

  void drawText(const double &top_left_x, const double &top_left_y, const string& text,
                const double &scale, Params params)
  {
      beginDrawingIfNeeded();
      Params msg;
      Vec2d top_left_xy = { top_left_x, top_left_y };
      msg["action"]="draw";
      msg["figure"]=params.pop("figure",current_fig);
      msg["shape"]=(params, "type","text",
                            "text",text,
                            "position",top_left_xy,
                            "scale", scale);
      fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
      fflush(channel.get());
  }

  void drawText(const double &top_left_x, const double &top_left_y, const string& text, Params params)
  {
      drawText(top_left_x,top_left_y,text,1.,params);
  }

  void drawVehicle(const double &cx, const double &cy, const double &rot, const double &length, Params params)
  {
      beginDrawingIfNeeded();
      Vec2d vc = { cx, cy };
      Params msg;
      msg["action"] = "draw";
      msg["figure"] = params.pop("figure",current_fig);
      msg["shape"] = (params, "type", "vehicle",
                              "center", vc,
                              "length", length,
                              "orientation", rot);

      fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
      fflush(channel.get());
  }

  void drawAUV(const double &cx, const double &cy, const double &rot, const double &length, Params params)
  {
      beginDrawingIfNeeded();
      Vec2d vc = { cx, cy };
      Params msg;
      msg["action"] = "draw";
      msg["figure"] = params.pop("figure",current_fig);
      msg["shape"] = (params, "type", "vehicle_auv",
                              "center", vc,
                              "length", length,
                              "orientation", rot);

      fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
      fflush(channel.get());
  }

  void drawMotorBoat(const double &cx, const double &cy, const double &rot, const double &length, Params params)
  {
      beginDrawingIfNeeded();
      Vec2d vc = { cx, cy };
      Params msg;
      msg["action"] = "draw";
      msg["figure"] = params.pop("figure",current_fig);
      msg["shape"] = (params, "type", "vehicle_motor_boat",
                              "center", vc,
                              "length", length,
                              "orientation", rot);

      fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
      fflush(channel.get());
  }

  void drawTank(const double &cx, const double &cy, const double &rot, const double &length, Params params)
  {
      beginDrawingIfNeeded();
      Vec2d vc = { cx, cy };
      Params msg;
      msg["action"] = "draw";
      msg["figure"] = params.pop("figure",current_fig);
      msg["shape"] = (params, "type", "vehicle_tank",
                              "center", vc,
                              "length", length,
                              "orientation", rot);

      fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
      fflush(channel.get());
  }

  void drawRaster(const std::string& rasterFilename, const double &xlb, const double &yub, const double &width, const double &height, Params params)
  {
    drawRaster(rasterFilename, xlb, yub, width, height, 0., params);
  }

  void drawRaster(const std::string& rasterFilename, const double &xlb, const double &yub, const double &width, const double &height, const double & rot, Params params)
  {
    beginDrawingIfNeeded();
    Vec2d ul_corner = { xlb, yub };
    Vec2d size = { width, height };

    Params msg;
    msg["action"] = "draw";
    msg["figure"] = params.pop("figure",current_fig);
    msg["shape"] = (params, "type", "raster",
                            "filename", rasterFilename,
                            "ul_corner", ul_corner,
                            "size", size,
                            "rot", rot
                   );

    fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
    fflush(channel.get());
  }

  void drawCake(const double &cx, const double &cy, const double &rot, const double &length, Params params)
  {
      beginDrawingIfNeeded();
      Vec2d vc = { cx, cy };
      Params msg;
      msg["action"] = "draw";
      msg["figure"] = params.pop("figure",current_fig);
      msg["shape"] = (params, "type", "cake",
                              "center", vc,
                              "length", length,
                              "orientation", rot);

      fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
      fflush(channel.get());
  }


  void newGroup(const std::string &name, Params params)
  {
     beginDrawingIfNeeded();
     // Send message
     Params msg;
     msg["action"] = "draw";
     msg["figure"] = params.pop("figure",current_fig);
     msg["shape"] = (params, "type", "group",
                             "name", name);

     fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
     fflush(channel.get());
  }

  void clearGroup(const std::string &figureName, const std::string &groupName)
  {
     beginDrawingIfNeeded();
     Params msg;
     msg["action"] = "clear";
     msg["figure"] = figureName;
     msg["group"] = groupName;

     fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
     fflush(channel.get());
  }

  void clearGroup(const std::string &groupName)
  {
     clearGroup(current_fig, groupName);
  }


  void removeObject(const std::string &figureName, const std::string &objectName)
  {
     beginDrawingIfNeeded();
     Params msg;
     msg["action"] = "delete";
     msg["figure"] = figureName;
     msg["object"] = objectName;

     fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
     fflush(channel.get());
  }

  void removeObject(const std::string &objectName)
  {
     removeObject(current_fig, objectName);
  }

  // Property modification
  void setFigureProperties(const std::string &figureName, const Params &properties)
  {
     beginDrawingIfNeeded();
     // Send message
     Params msg;
     msg["action"] = "set";
     msg["figure"] = figureName;
     msg["properties"] = properties;

     fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
     fflush(channel.get());
  }

  void setFigureProperties(const Params &properties)
  {
    beginDrawingIfNeeded();
     setFigureProperties(current_fig, properties);
  }

  void setObjectProperties(const std::string &figureName, const std::string &objectName, const Params &properties)
  {
     beginDrawingIfNeeded();
     // Send message
     Params msg;
     msg["action"] = "set";
     msg["figure"] = figureName;
     msg["object"] = objectName;
     msg["properties"] = properties;

     fputs(Value(msg).toJSONString().append("\n\n").c_str(), channel.get());
     fflush(channel.get());
  }

  void setObjectProperties(const std::string &objectName, const Params &properties)
  {
     setObjectProperties(current_fig, objectName, properties);
  }
}