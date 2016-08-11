/* Copyright (c) 2016 Fabian Schuiki */
#pragma once
#include <asic/lef.h>
#include "common.h"
#include "util.h"


/**
 * @defgroup lef LEF Library
 * A library that reads and writes LEF files.
 * @{
 */



// -----------------------------------------------------------------------------
// File
// -----------------------------------------------------------------------------
struct lef {
	char *version;
	array_t sites;
	array_t macros; // lef_macro_t*
};



// -----------------------------------------------------------------------------
// Macro
// -----------------------------------------------------------------------------

/// Bits that make up the symmetry mask of a macro.
enum lef_macro_symmetry {
	LEF_MACRO_SYM_X   = 1 << 0,
	LEF_MACRO_SYM_Y   = 1 << 1,
	LEF_MACRO_SYM_R90 = 1 << 2,
};

/// A macro statement within a LEF file.
struct lef_macro {
	/// Name of the macro.
	char *name;
	/// Origin point of the macro, in meters.
	lef_xy_t origin;
	/// Size of the macro, in meters.
	lef_xy_t size;
	/// Symmetry flags of the macro. See `enum lef_macro_symmetry` for details.
	uint8_t symmetry;
	/// Pins of this macro. See lef_pin for details.
	array_t pins;  // lef_pin_t*
	/// Obstruction geometries of this macro. See lef_geo for details.
	array_t obs; // lef_geo_t*
};



// -----------------------------------------------------------------------------
// Pin
// -----------------------------------------------------------------------------

/// Pin direction
enum lef_pin_direction {
	LEF_PIN_DIR_INPUT = 1,
	LEF_PIN_DIR_OUTPUT,
	LEF_PIN_DIR_TRISTATE,
	LEF_PIN_DIR_INOUT,
	LEF_PIN_DIR_FEEDTHRU,
};

/// Pin use
enum lef_pin_use {
	LEF_PIN_USE_SIGNAL = 1,
	LEF_PIN_USE_ANALOG,
	LEF_PIN_USE_POWER,
	LEF_PIN_USE_GROUND,
	LEF_PIN_USE_CLOCK,
};

/// Pin shape
enum lef_pin_shape {
	LEF_PIN_SHAPE_ABUTMENT = 1,
	LEF_PIN_SHAPE_RING,
	LEF_PIN_SHAPE_FEEDTHRU,
};

/// A pin statement within a macro.
struct lef_pin {
	/// Name of the pin.
	char *name;
	/// Direction of the pin.
	enum lef_pin_direction direction;
	/// Use of the pin.
	enum lef_pin_use use;
	/// Shape of the pin.
	enum lef_pin_shape shape;
	/// What other pin this pin must join with.
	char *must_join;
	/// The ports of this pin. See lef_port for details.
	array_t ports; // lef_port_t*
};

/// Port class
enum lef_port_class {
	LEF_PORT_CLASS_NONE = 1,
	LEF_PORT_CLASS_CORE,
	LEF_PORT_CLASS_BUMP,
};

/// A port statement within a pin.
struct lef_port {
	/// Class of the port.
	enum lef_port_class cls;
	/// Geometries of this port. See lef_geo for details.
	array_t geos; /// lef_geo_t*
	/// Last geometry layer that was added to the port. Used internally by the
	/// parser.
	struct lef_geo_layer *last_layer;
};



// -----------------------------------------------------------------------------
// Geometry
// -----------------------------------------------------------------------------

enum lef_geo_kind {
	LEF_GEO_LAYER,
	LEF_GEO_VIA,
};

struct lef_geo {
	enum lef_geo_kind kind;
};

struct lef_geo_layer {
	lef_geo_t geo;
	char *layer;
	double min_spacing;
	double design_rule_width;
	double width;
	array_t shapes; /* lef_geo_shape_t* */
};

struct lef_geo_via {
	struct lef_geo geo;
	char *name;
	int32_t mask;
	lef_xy_t pos;
	lef_geo_iterate_t *iterate;
};

enum lef_geo_shape_kind {
	LEF_SHAPE_PATH,
	LEF_SHAPE_RECT,
	LEF_SHAPE_POLYGON,
};

struct lef_geo_shape {
	enum lef_geo_shape_kind kind;
	int32_t mask;
	uint16_t num_points;
	lef_xy_t *points;
	lef_geo_iterate_t *iterate;
};



/** @} lef */
