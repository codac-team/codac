
/** 
 *  ile
 *  VIBesFigTubeVector documentation header 
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  uthor     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */
 
 const char* DOCS_VIBESFIGTUBEVECTOR=R"_docs(/**
* \class VIBesFigTubeVector
* \brief Multi-view item to display vector tubes or trajectories
*
* A set of figures is linked to some tube vector or trajectory pointers, so that
* any update on these objects can be easily displayed on the figures.
*
* There will be as many windows as dimensions to be represented.
*
* \todo test this class
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_VIBESFIGTUBEVECTOR_STRING=R"_docs(/// \name Basics
/// @{/**
* \brief Creates a VIBesFigTubeVector
*
* \param fig_name name of the figure as displayed in the window title
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_VIBESFIGTUBEVECTOR_STRING_INT_INT=R"_docs(/**
* \brief Creates a VIBesFigTubeVector
*
* \param fig_name name of the figure as displayed in the window title
* \param start_index the first dimension to be displayed
* \param end_index the last dimension to be displayed
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_VIBESFIGTUBEVECTOR_STRING_TUBEVECTOR_TRAJECTORYVECTOR=R"_docs(/**
* \brief Creates a VIBesFigTube
*
* The dimension of the figure (number of windows) is based on the `tubevector` parameter.
*
* \param fig_name name of the figure as displayed in the window title
* \param tubevector a const pointer to the tube vector to be displayed
* \param trajvector an optional const pointer to a trajectory to be displayed (`NULL` by default)
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_SIZE=R"_docs(/**
* \brief Returns the dimension \f$n\f$ of the objects displayed in the figure
*
* \note Some dimensions may not be displayed (see the subfigs_number method)
*
* \return n
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_SUBFIGS_NUMBER=R"_docs(/**
* \brief Returns number of dimensions to be displayed
*
* This number corresponds to the number of windows to be created.
*
* \return the number of subfigures
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_OPERATOR_INDEX_INT=R"_docs(/**
* \brief Returns a pointer to the ith VIBesFigTube of this figure
*
* \param index the index of this ith component
* \return a pointer to the ith component
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_OPERATOR_INDEX_INT1=R"_docs(/**
* \brief Returns a const pointer to the ith VIBesFigTube of this figure
*
* \param index the index of this ith component
* \return a const pointer to the ith component
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_SET_PROPERTIES_INT_INT_INT_INT=R"_docs(/**
* \brief Sets the properties (coordinates and dimensions) of this figure
*
* \param x horizontal coordinate (in pixels)
* \param y vertical coordinate (in pixels)
* \param width width value (in pixels)
* \param height height value (in pixels)
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_SHOW_BOOL=R"_docs(/**
* \brief Displays this figure with optional details
*
* \param detail_slices if `true`, each slice will be displayed as a box,
*        otherwise, only polygon envelopes of the tubes will be shown (fast display)
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_SET_CURSOR_DOUBLE=R"_docs(/**
* \brief Set the position of the temporal cursor of this figure
*
* \param t position on the temporal domain
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_SHOW_CURSOR_BOOL=R"_docs(/**
* \brief Displays the cursor on this figure
*
* \param display parameter (`true` by default)
*/
)_docs";

// const char* DOCS_VIBESFIGTUBEVECTOR_DRAW_BOX_INTERVAL_INTERVALVECTOR_VIBES::PARAMS=R"_docs(/// @}
// /// \name Simple drawings
// /// @{/**
// * \brief Draws a multi-dimensional box
// *
// * \param domain the temporal domain related to the box
// * \param box the \f$n\f$d IntervalVector to be displayed
// * \param params VIBes parameters related to the box
// */
// )_docs";

// const char* DOCS_VIBESFIGTUBEVECTOR_DRAW_BOX_INTERVAL_INTERVALVECTOR_STRING_VIBES::PARAMS=R"_docs(/**
// * \brief Draws a multi-dimensional box
// *
// * \param domain the temporal domain related to the box
// * \param box the \f$n\f$d IntervalVector to be displayed
// * \param color the optional color of the box (black by default) 
// * \param params VIBes parameters related to the box (none by default)
// */
// )_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_ADD_TUBE_TUBEVECTOR_STRING_STRING_STRING=R"_docs(/// @}
/// \name Handling tubes
/// @{/**
* \brief Adds a tube vector object to this figure
*
* \param tubevector a const pointer to a TubeVector object to be displayed
* \param name a name to identify this object
* \param color_frgrnd an optional color for the current values of the tube
* \param color_bckgrnd an optional color for the previous values of the tube, before any new contraction
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_SET_TUBE_NAME_TUBEVECTOR_STRING=R"_docs(/**
* \brief Sets a new name for a tube vector
*
* \param tubevector the const pointer to the TubeVector object to be renamed
* \param name a new name to identify this object
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_SET_TUBE_DERIVATIVE_TUBEVECTOR_TUBEVECTOR=R"_docs(/**
* \brief Links a tube vector to its derivative for display purposes
*
* When available, a derivative tube \f$[\dot{\mathbf{x}}](\cdot)\f$ allows to draw slices of
* \f$[\mathbf{x}](\cdot)\f$ as polygons, thus displaying a thinner envelope of the tube \f$[\mathbf{x}](\cdot)\f$. 
*
* \param tubevector the const pointer to the TubeVector object that will be displayed
* \param derivative the const pointer its derivative set
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_SET_TUBE_COLOR_TUBEVECTOR_STRING_STRING=R"_docs(/**
* \brief Sets color properties for a given tube vector
*
* \param tubevector the const pointer to the TubeVector object for which the colors will be set
* \param color_frgrnd a color for the current values of the tube
* \param color_bckgrnd a color for the previous values of the tube, before any new contraction (gray by default)
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_SET_TUBE_COLOR_TUBEVECTOR_TUBECOLORTYPE_STRING=R"_docs(/**
* \brief Sets color properties for a given tube vector
*
* This method allows to change the display of slices, borders, gates, etc.
*
* \param tubevector the const pointer to the TubeVector object for which the colors will be set
* \param color_type the `TubeColorType` key for which the value will be set
* \param color the new color to be specified for the given type
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_RESET_TUBE_BACKGROUND_TUBEVECTOR=R"_docs(/**
* \brief Reset the background of a given tube vector
*
* \todo automatically clear the figure (for now, the pointer is only deleted)
*
* \param tubevector the const pointer to the TubeVector object for which the background will be deleted
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_REMOVE_TUBE_TUBEVECTOR=R"_docs(/**
* \brief Removes a tube vector from this figure
*
* \todo automatically clear the figure (for now, the tube is only removed
*       from the list of objects to be shown afterwards)
*
* \note the object will not be deleted
*
* \param tubevector the const pointer to the TubeVector object to be removed
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_ADD_TRAJECTORY_TRAJECTORYVECTOR_STRING_STRING=R"_docs(/// @}
/// \name Handling trajectories
/// @{/**
* \brief Adds a trajectory vector object to this figure
*
* \param trajvector a const pointer to a TrajectoryVector object to be displayed
* \param name a name to identify this object
* \param color an optional color to draw this trajectory
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_SET_TRAJECTORY_NAME_TRAJECTORYVECTOR_STRING=R"_docs(/**
* \brief Sets a new name for a trajectory vector
*
* \param trajvector the const pointer to the TrajectoryVector object to be renamed
* \param name a new name to identify this object
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_SET_TRAJECTORY_COLOR_TRAJECTORYVECTOR_STRING=R"_docs(/**
* \brief Sets color properties for a given trajectory vector
*
* \param trajvector the const pointer to the TrajectoryVector object for which the color will be set
* \param color a color to draw this trajectory
*/
)_docs";

const char* DOCS_VIBESFIGTUBEVECTOR_REMOVE_TRAJECTORY_TRAJECTORYVECTOR=R"_docs(/**
* \brief Removes a trajectory vector from this figure
*
* \todo automatically clear the figure (for now, the trajectory is only removed
*       from the list of objects to be shown afterwards)
*
* \note the object will not be deleted
*
* \param trajvector the const pointer to the TrajectoryVector object to be removed
*/
)_docs";

