
/** 
 *  ile
 *  VIBesFigTube documentation header 
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  uthor     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */
 
 const char* DOCS_TUBECOLORTYPE=R"_docs(
)_docs";

const char* DOCS_VIBESFIGTUBE=R"_docs(/**
* \class VIBesFigTube
* \brief Two-dimensional graphical item to display scalar tubes or trajectories
*
* One figure is linked to some tube or trajectory pointers, so that
* any update on these objects can be easily displayed on the figure. 
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_VIBESFIGTUBE_STRING_TUBE_TRAJECTORY=R"_docs(/// \name Basics
/// @{/**
* \brief Creates a VIBesFigTube
*
* \param fig_name name of the figure as displayed in the window title
* \param tube an optional const pointer to the tube to be displayed (`NULL` by default)
* \param traj an optional const pointer to a trajectory to be displayed (`NULL` by default)
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_SHOW=R"_docs(/**
* \brief Displays this figure
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_SHOW_BOOL=R"_docs(/**
* \brief Displays this figure with optional details
*
* \param detail_slices if `true`, each slice will be displayed as a box,
*        otherwise, only polygon envelopes of the tubes will be shown (fast display)
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_SET_CURSOR_DOUBLE=R"_docs(/**
* \brief Set the position of the temporal cursor of this figure
*
* \param t position on the temporal domain
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_SHOW_CURSOR_BOOL=R"_docs(/**
* \brief Displays the cursor on this figure
*
* \param display parameter (`true` by default)
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_ADD_TUBE_TUBE_STRING_STRING_STRING=R"_docs(/// @}
/// \name Handling tubes
/// @{/**
* \brief Adds a tube object to this figure
*
* \param tube a const pointer to a Tube object to be displayed
* \param name a name to identify this object
* \param color_frgrnd an optional color for the current values of the tube
* \param color_bckgrnd an optional color for the previous values of the tube, before any new contraction
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_ADD_TUBES_TUBEVECTOR_STRING_STRING_STRING=R"_docs(/**
* \brief Adds a subset of a vector of tubes to this figure
* 
* Each selected component will be projected on the same figure.
*
* \param tubevector a const pointer to a TubeVector object to be displayed
* \param name a name to identify this set of tubes
* \param color_frgrnd an optional color for the current values of the tubes
* \param color_bckgrnd an optional color for the previous values of the tubes, before any new contraction
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_ADD_TUBES_TUBEVECTOR_INT_INT_STRING_STRING_STRING=R"_docs(/**
* \brief Adds a subset of a vector of tubes to this figure
* 
* Each selected component will be projected on the same figure.
*
* \param tubevector a const pointer to a TubeVector object to be displayed
* \param start_index the first dimension to be displayed
* \param end_index the last dimension to be displayed
* \param name a name to identify this set of tubes
* \param color_frgrnd an optional color for the current values of the tubes
* \param color_bckgrnd an optional color for the previous values of the tubes, before any new contraction
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_SET_TUBE_NAME_TUBE_STRING=R"_docs(/**
* \brief Sets a new name for a tube
*
* \param tube the const pointer to the Tube object to be renamed
* \param name a new name to identify this object
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_SET_TUBE_DERIVATIVE_TUBE_TUBE=R"_docs(/**
* \brief Links a tube to its derivative for display purposes
*
* When available, a derivative tube \f$[\dot{x}](\cdot)\f$ allows to draw slices of
* \f$[x](\cdot)\f$ as polygons, thus displaying a thinner envelope of the tube \f$[x](\cdot)\f$. 
*
* \param tube the const pointer to the Tube object that will be displayed
* \param derivative the const pointer its derivative set
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_SET_TUBE_COLOR_TUBE_STRING_STRING=R"_docs(/**
* \brief Sets color properties for a given tube
*
* \param tube the const pointer to the Tube object for which the colors will be set
* \param color_frgrnd a color for the current values of the tube
* \param color_bckgrnd a color for the previous values of the tube, before any new contraction (gray by default)
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_SET_TUBE_COLOR_TUBE_TUBECOLORTYPE_STRING=R"_docs(/**
* \brief Sets color properties for a given tube
*
* This method allows to change the display of slices, borders, gates, etc.
*
* \param tube the const pointer to the Tube object for which the colors will be set
* \param color_type the `TubeColorType` key for which the value will be set
* \param color the new color to be specified for the given type
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_RESET_TUBE_BACKGROUND_TUBE=R"_docs(/**
* \brief Reset the background of a given tube
*
* \todo automatically clear the figure (for now, the pointer is only deleted)
*
* \param tube the const pointer to the Tube object for which the background will be deleted
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_REMOVE_TUBE_TUBE=R"_docs(/**
* \brief Removes a tube from this figure
*
* \todo automatically clear the figure (for now, the tube is only removed
*       from the list of objects to be shown afterwards)
*
* \note the object will not be deleted
*
* \param tube the const pointer to the Tube object to be removed
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_ADD_TRAJECTORY_TRAJECTORY_STRING_STRING=R"_docs(/// @}
/// \name Handling trajectories
/// @{/**
* \brief Adds a trajectory object to this figure
*
* \param traj a const pointer to a Trajectory object to be displayed
* \param name a name to identify this object
* \param color an optional color to draw this trajectory
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_ADD_TRAJECTORIES_TRAJECTORYVECTOR_STRING_STRING=R"_docs(/**
* \brief Adds a vector of trajectories to this figure
* 
* Each component will be projected on the same figure.
*
* \param trajvector a const pointer to a TrajectoryVector object to be displayed
* \param name a name to identify this set of trajectories
* \param color an optional color to draw these trajectories
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_ADD_TRAJECTORIES_TRAJECTORYVECTOR_INT_INT_STRING_STRING=R"_docs(/**
* \brief Adds a subset of a vector of trajectories to this figure
* 
* Each selected component will be projected on the same figure.
*
* \param trajvector a const pointer to a TrajectoryVector object to be displayed
* \param start_index the first dimension to be displayed
* \param end_index the last dimension to be displayed
* \param name a name to identify this set of trajectories
* \param color an optional color to draw these trajectories
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_SET_TRAJECTORY_NAME_TRAJECTORY_STRING=R"_docs(/**
* \brief Sets a new name for a trajectory
*
* \param traj the const pointer to the Trajectory object to be renamed
* \param name a new name to identify this object
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_SET_TRAJECTORY_COLOR_TRAJECTORY_STRING=R"_docs(/**
* \brief Sets color properties for a given trajectory
*
* \param traj the const pointer to the Trajectory object for which the color will be set
* \param color a color to draw this trajectory
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_SET_TRAJECTORY_POINTS_SIZE_TRAJECTORY_FLOAT=R"_docs(/**
* \brief Sets points size for a given trajectory, and activates
*        a display mode with points instead of a line for this trajectory
*
* \param traj the const pointer to the Trajectory object for which the points size will be set
* \param points_size size of the points (if 0, the display is done with a line)
*/
)_docs";

const char* DOCS_VIBESFIGTUBE_REMOVE_TRAJECTORY_TRAJECTORY=R"_docs(/**
* \brief Removes a trajectory from this figure
*
* \todo automatically clear the figure (for now, the trajectory is only removed
*       from the list of objects to be shown afterwards)
*
* \note the object will not be deleted
*
* \param traj the const pointer to the Trajectory object to be removed
*/
)_docs";

const char* DOCS_FIGTUBEPARAMS=R"_docs(/**
* \struct FigTubeParams
* \brief Specifies some parameters related to a Tube display
*/
)_docs";

const char* DOCS_FIGTRAJPARAMS=R"_docs(//!< to display thinner envelopes (polygons) enclosed by the slices/**
* \struct FigTrajParams
* \brief Specifies some parameters related to a Trajectory display
*/
)_docs";

