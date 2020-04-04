
/** 
 *  ile
 *  VIBesFig documentation header 
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  uthor     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */
 
 const char* DOCS_VIBESFIG=R"_docs(/**
* \class VIBesFig
* \brief Two-dimensional graphical item based on the VIBes viewer
*/
)_docs";

const char* DOCS_VIBESFIG_VIBESFIG_STRING=R"_docs(/// \name Definition and properties
/// @{/**
* \brief Creates a VIBesFig
*
* \param fig_name a reference to the figure that will be displayed in the window's title
*/
)_docs";

const char* DOCS_VIBESFIG_CLOSE=R"_docs(/**
* \brief Closes this figure
*/
)_docs";

const char* DOCS_VIBESFIG_SET_PROPERTIES_INT_INT_INT_INT=R"_docs(/**
* \brief Sets the properties (coordinates and dimensions) of this figure
*
* \param x horizontal coordinate (in pixels)
* \param y vertical coordinate (in pixels)
* \param width width value (in pixels)
* \param height height value (in pixels)
*/
)_docs";

const char* DOCS_VIBESFIG_SET_BACKGROUND_STRING=R"_docs(/**
* \brief Sets the background color of this figure
*
* \param bg_color color of the background
*/
)_docs";

const char* DOCS_VIBESFIG_AXIS_LIMITS_DOUBLE_DOUBLE_DOUBLE_DOUBLE_BOOL_FLOAT=R"_docs(/**
* \brief Sets the axis limits of this figure
*
* The function updates the viewbox and applies the changes.
*
* \param x_min lower horizontal value to be displayed
* \param x_max upper horizontal value to be displayed
* \param y_min lower vertical value to be displayed
* \param y_max upper vertical value to be displayed
* \param same_ratio if `true`, will compute the min/max values so
*        that the previous ratio will be preserved (false by default)
* \param margin adds a custom margin to the view box (none by default)
* \return the updated view box of this figure
*/
)_docs";

const char* DOCS_VIBESFIG_AXIS_LIMITS_INTERVALVECTOR_BOOL_FLOAT=R"_docs(/**
* \brief Sets the axis limits of this figure
*
* The function updates the viewbox and applies the changes.
*
* \param viewbox the 2d box defining lower/upper horizontal/vertical values
* \param same_ratio if `true`, will compute the min/max values so
*        that the previous ratio will be preserved (false by default)
* \param margin adds a custom margin to the view box (none by default)
* \return the updated view box of this figure
*/
)_docs";

const char* DOCS_VIBESFIG_SAVE_IMAGE_STRING_STRING_STRING=R"_docs(/// @}
/// \name Saving this figure
/// @{/**
* \brief Saves the figure in SVG/PNG/... format
*
* A file named {path}/{figure_name}{suffix}.{extension} will be created in the current directory.
*
* \param suffix optional part name that can be added to the figure name (none by default)
* \param extension optional part to specify the type of the image ("svg" by default)
* \param path optional path to a different directory ("." by default)
*/
)_docs";

const char* DOCS_VIBESFIG_SHOW=R"_docs(/// @}
/// \name Figure's content
/// @{/**
* \brief Displays this figure
*/
)_docs";

const char* DOCS_VIBESFIG_CLEAR=R"_docs(/**
* \brief Clears this figure by removing displayed items
*/
)_docs";


