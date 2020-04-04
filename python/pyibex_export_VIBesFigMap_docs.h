
/** 
 *  ile
 *  VIBesFigMap documentation header 
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  uthor     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */
 
 const char* DOCS_VIBESFIGMAP=R"_docs(/**
* \class VIBesFigMap
* \brief Two-dimensional graphical item to project dynamical items
*        (tubes, trajectories, etc.) on a map.
*
* One figure is linked to some tube or trajectory pointers, so that
* any update on these objects can be easily displayed on the figure. 
*/
)_docs";

const char* DOCS_VIBESFIGMAP_VIBESFIGMAP_STRING=R"_docs(/// \name Basics
/// @{/**
* \brief Creates a VIBesFigMap
*
* \param fig_name name of the figure as displayed in the window title
*/
)_docs";

const char* DOCS_VIBESFIGMAP_SET_RESTRICTED_TDOMAIN_INTERVAL=R"_docs(/**
* \brief Restricts the display of the dynamical items to a part
*        of their temporal domain only
*
* \param restricted_tdomain subset of the temporal domain of the referenced items
*/
)_docs";

const char* DOCS_VIBESFIGMAP_ENABLE_TUBES_BACKGROUNDS_BOOL=R"_docs(/**
* \brief Enables the display of previous versions of the tubes,
*        in order to highlight the contractions of these sets
*
* \param enable boolean parameter, `true` by default
*/
)_docs";

const char* DOCS_VIBESFIGMAP_SHOW=R"_docs(/**
* \brief Displays this figure
*/
)_docs";

const char* DOCS_VIBESFIGMAP_SHOW_FLOAT=R"_docs(/**
* \brief Displays this figure with a custom robot size
*
* \param robot_size length of the displayed robot
*/
)_docs";

const char* DOCS_VIBESFIGMAP_SET_TUBE_MAX_DISP_SLICES_INT=R"_docs(/**
* \brief Limits the number of slices to be displayed for tubes
*
* \param max the maximum number of slices
*/
)_docs";

const char* DOCS_VIBESFIGMAP_SET_TRAJ_MAX_DISP_POINTS_INT=R"_docs(/**
* \brief Limits the number of points to be displayed for trajectories
*
* Note that this will be applied for trajectories defined
* from tubex::Function objects (discretization).
*
* \param max the maximum number of points
*/
)_docs";

const char* DOCS_VIBESFIGMAP_SMOOTH_TUBE_DRAWING_BOOL=R"_docs(/**
* \brief Enables the smoothing of tubes
*
* Instead of boxed slices, polygons will be drawn which will provide
* a smooth but non-reliable rendering.
*
* \param smooth `true` for smooth display
*/
)_docs";

const char* DOCS_VIBESFIGMAP_ADD_TUBE_TUBEVECTOR_STRING_INT_INT=R"_docs(/// @}
/// \name Handling tubes
/// @{/**
* \brief Adds a tube vector object to this figure
*
* \param tube a const pointer to a TubeVector object to be displayed
* \param name a name to identify this object
* \param index_x integer reference for the x-horizontal state component
* \param index_y integer reference for the y-horizontal state component
*/
)_docs";

const char* DOCS_VIBESFIGMAP_SET_TUBE_NAME_TUBEVECTOR_STRING=R"_docs(/**
* \brief Sets a new name for a tube
*
* \param tube the const pointer to the TubeVector object to be renamed
* \param name a new name to identify this object
*/
)_docs";

const char* DOCS_VIBESFIGMAP_SET_TUBE_COLOR_TUBEVECTOR_STRING=R"_docs(/**
* \brief Sets constant color properties for a given tube
*
* \param tube the const pointer to the TubeVector object for which the color will be set
* \param color a color to draw this tube
*/
)_docs";

const char* DOCS_VIBESFIGMAP_SET_TUBE_COLOR_TUBEVECTOR_COLORMAP_TRAJECTORY=R"_docs(/**
* \brief Sets a color map for a given tube
*
* \param tube the const pointer to the TubeVector object for which the color will be set
* \param colormap a ColorMap to draw this tube with time evolving colors
* \param traj_colormap an optional const pointer to a Trajectory object in order to have a custom
*        color evolution instead of a linear time evolving color 
*/
)_docs";

const char* DOCS_VIBESFIGMAP_REMOVE_TUBE_TUBEVECTOR=R"_docs(/**
* \brief Removes a tube from this figure
*
* \todo automatically clear the figure (for now, the tube is only removed
*       from the list of objects to be shown afterwards)
*
* \note the object will not be deleted
*
* \param tube the const pointer to the TubeVector object to be removed
*/
)_docs";

const char* DOCS_VIBESFIGMAP_ADD_TRAJECTORY_TRAJECTORYVECTOR_STRING_INT_INT_STRING=R"_docs(/// @}
/// \name Handling trajectories
/// @{/**
* \brief Adds a trajectory vector object (x,y) to this figure
*
* The heading information may be required for vehicle display. Here it will be simulated from
* the two components of the traj object. If available, use the other add_trajectory method.
*
* \param traj a const pointer to a TrajectoryVector object to be displayed
* \param name a name to identify this object
* \param index_x integer reference for the x-horizontal state component
* \param index_y integer reference for the y-horizontal state component
* \param color an optional constant color for this object
*/
)_docs";

const char* DOCS_VIBESFIGMAP_ADD_TRAJECTORY_TRAJECTORYVECTOR_STRING_INT_INT_INT_STRING=R"_docs(/**
* \brief Adds a trajectory vector object (x,y,heading) to this figure
*
* The heading information may be required for vehicle display. 
* If not available, use the other add_trajectory method.
*
* \param traj a const pointer to a TrajectoryVector object to be displayed
* \param name a name to identify this object
* \param index_x integer reference for the x-horizontal state component
* \param index_y integer reference for the y-horizontal state component
* \param index_heading integer reference for the heading state component
* \param color an optional constant color for this object
*/
)_docs";

const char* DOCS_VIBESFIGMAP_SET_TRAJECTORY_NAME_TRAJECTORYVECTOR_STRING=R"_docs(/**
* \brief Sets a new name for a trajectory
*
* \param traj the const pointer to the TrajectoryVector object to be renamed
* \param name a new name to identify this object
*/
)_docs";

const char* DOCS_VIBESFIGMAP_SET_TRAJECTORY_COLOR_TRAJECTORYVECTOR_STRING=R"_docs(/**
* \brief Sets constant color properties for a given trajectory
*
* \param traj the const pointer to the TrajectoryVector object for which the color will be set
* \param color a color to draw this trajectory
*/
)_docs";

const char* DOCS_VIBESFIGMAP_SET_TRAJECTORY_COLOR_TRAJECTORYVECTOR_COLORMAP_TRAJECTORY=R"_docs(/**
* \brief Sets a color map for a given trajectory
*
* \param traj the const pointer to the TrajectoryVector object for which the color will be set
* \param colormap a ColorMap to draw this trajectory with time evolving colors
* \param traj_colormap an optional const pointer to a Trajectory object in order to have a custom
*        color evolution instead of a linear time evolving color 
*/
)_docs";

const char* DOCS_VIBESFIGMAP_REMOVE_TRAJECTORY_TRAJECTORYVECTOR=R"_docs(/**
* \brief Removes a trajectory from this figure
*
* \todo automatically clear the figure (for now, the trajectory is only removed
*       from the list of objects to be shown afterwards)
*
* \note the object will not be deleted
*
* \param traj the const pointer to the TrajectoryVector object to be removed
*/
)_docs";

const char* DOCS_VIBESFIGMAP_DRAW_VEHICLE_VECTOR_FLOAT=R"_docs(/// @}
/// \name Handling robotics objects
/// @{/**
* \brief Draws a vehicle with a given pose
*
* \param pose vector (x,y[,heading]) describing robot's pose (2d or 3d)
* \param size optional robot size (-1 = size of main vehicle by default)
*/
)_docs";

// const char* DOCS_VIBESFIGMAP_DRAW_VEHICLE_VECTOR_VIBES::PARAMS_FLOAT=R"_docs(/**
// * \brief Draws a vehicle with a given pose
// *
// * \param pose vector (x,y[,heading]) describing robot's pose (2d or 3d)
// * \param params VIBes parameters related to the vehicle (for groups)
// * \param size optional robot size (-1 = size of main vehicle by default)
// */
// )_docs";

const char* DOCS_VIBESFIGMAP_DRAW_VEHICLE_DOUBLE_TRAJECTORYVECTOR_FLOAT=R"_docs(/**
* \brief Draws a vehicle on top of its trajectory
*
* \param t temporal key of the vehicle state to be drawn
* \param traj the const pointer to the related TrajectoryVector object
* \param size optional robot size (-1 = size of main vehicle by default)
*/
)_docs";

// const char* DOCS_VIBESFIGMAP_DRAW_VEHICLE_DOUBLE_TRAJECTORYVECTOR_VIBES::PARAMS_FLOAT=R"_docs(/**
// * \brief Draws a vehicle on top of its trajectory
// *
// * \param t temporal key of the vehicle state to be drawn
// * \param traj the const pointer to the related TrajectoryVector object
// * \param params VIBes parameters related to the vehicle (for groups)
// * \param size optional robot size (-1 = size of main vehicle by default)
// */
// )_docs";

const char* DOCS_VIBESFIGMAP_ADD_BEACON_BEACON_STRING=R"_docs(/**
* \brief Adds a Beacon object to the map
*
* The width of the displayed box is related to the uncertainty of the Beacon object.
*
* \param beacon a const reference to the Beacon object to be drawn
* \param color optional color of the beacon
*/
)_docs";

const char* DOCS_VIBESFIGMAP_ADD_BEACON_BEACON_DOUBLE_STRING=R"_docs(/**
* \brief Adds a Beacon object to the map with a specific width
*
* \param beacon a const reference to the Beacon object to be drawn
* \param width the real width of the squared beacon
* \param color optional color of the beacon
*/
)_docs";

const char* DOCS_VIBESFIGMAP_ADD_BEACONS_VECTOR_BEACON__STRING=R"_docs(/**
* \brief Adds a set of Beacon objects to the map
*
* The width of the displayed boxes is related to the uncertainty of the Beacon objects.
*
* \deprecated to be removed
*
* \param v_beacons a vector of const references to Beacon objects to be drawn
* \param color optional color of the beacons
*/
)_docs";

const char* DOCS_VIBESFIGMAP_ADD_BEACONS_VECTOR_BEACON__DOUBLE_STRING=R"_docs(/**
* \brief Adds a set of Beacon objects to the map with a specific width
*
* \deprecated to be removed
*
* \param v_beacons a vector of const references to Beacon objects to be drawn
* \param width the real width of the squared beacons
* \param color optional color of the beacons
*/
)_docs";

const char* DOCS_VIBESFIGMAP_ADD_LANDMARKS_VECTOR_INTERVALVECTOR__STRING=R"_docs(/**
* \brief Adds a set of boxed landmarks to the map
*
* \param v_m a vector of boxes to be drawn
* \param color optional color of the beacons
*/
)_docs";

const char* DOCS_VIBESFIGMAP_ADD_LANDMARKS_VECTOR_VECTOR__DOUBLE_STRING=R"_docs(/**
* \brief Adds a set of landmarks to the map with a specific width
*
* \param v_m a vector of 2d positions of landmarks
* \param width the width of the squared beacons
* \param color optional color of the beacons
*/
)_docs";

const char* DOCS_VIBESFIGMAP_ADD_OBSERVATION_INTERVALVECTOR_VECTOR_STRING=R"_docs(/**
* \brief Adds a range-and-bearing uncertain observation to the map,
*        in a static context
*
* The observation is a 2d interval vector: (range,bearing).
* It is associated with the pose of a robot, from which the observation has been made.
*
* \param obs the 2d interval vector enclosing the measurement
* \param pose the related 3d Vector object representing the pose (x,y,heading)
* \param color optional color of the observation
*/
)_docs";

const char* DOCS_VIBESFIGMAP_ADD_OBSERVATION_INTERVALVECTOR_TRAJECTORYVECTOR_STRING=R"_docs(/**
* \brief Adds a range-and-bearing uncertain observation to the map,
*        in a dynamic context
*
* The observation is a 3d interval vector: (time,range,bearing).
* It is associated with the trajectory of a robot, from which the observation has been made.
*
* \param obs the 3d interval vector enclosing the measurement
* \param traj the const pointer to the related TrajectoryVector object
* \param color optional color of the observation
*/
)_docs";

const char* DOCS_VIBESFIGMAP_ADD_OBSERVATIONS_VECTOR_INTERVALVECTOR__VECTOR_STRING=R"_docs(/**
* \brief Adds a set of range-and-bearing uncertain observations to the map,
*        in a static context
*
* The observation is a 2d interval vector: (range,bearing).
* It is associated with the pose of a robot, from which the observation has been made.
*
* \param v_obs a vector of 2d interval vectors enclosing the measurements
* \param pose the related 3d Vector object representing the pose (x,y,heading)
* \param color optional color of the observation
*/
)_docs";

const char* DOCS_VIBESFIGMAP_ADD_OBSERVATIONS_VECTOR_INTERVALVECTOR__TRAJECTORYVECTOR_STRING=R"_docs(/**
* \brief Adds a set of range-and-bearing uncertain observations to the map,
*        in a dynamic context
*
* The observation is a 3d interval vector: (time,range,bearing).
* It is associated with the trajectory of a robot, from which the observation has been made.
*
* \param v_obs a vector of 3d interval vectors enclosing the measurements
* \param traj the const pointer to the related TrajectoryVector object
* \param color optional color of the observation
*/
)_docs";

const char* DOCS_FIGMAPTUBEPARAMS=R"_docs(/**
* \struct FigMapTubeParams
* \brief Specifies some parameters related to a Tube display
*/
)_docs";

const char* DOCS_FIGMAPTRAJPARAMS=R"_docs(//!< integer references for state components/**
* \struct FigMapTrajParams
* \brief Specifies some parameters related to a Trajectory display
*/
)_docs";

