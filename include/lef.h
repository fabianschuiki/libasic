${HEADER_PREFIX}
#include <stddef.h>


// LEF library version
#define LEF_VERSION_MAJOR @ASIC_VERSION_MAJOR@
#define LEF_VERSION_MINOR @ASIC_VERSION_MINOR@
#define LEF_VERSION_PATCH @ASIC_VERSION_PATCH@
#define LEF_VERSION_STRING "@ASIC_VERSION@"


// Definition of types used throughout the library.
typedef struct lef lef_t;
typedef struct lef_geo lef_geo_t;
typedef struct lef_geo_iterate lef_geo_iterate_t;
typedef struct lef_geo_layer lef_geo_layer_t;
typedef struct lef_geo_shape lef_geo_shape_t;
typedef struct lef_geo_via lef_geo_via_t;
typedef struct lef_macro lef_macro_t;
typedef struct lef_pin lef_pin_t;
typedef struct lef_port lef_port_t;
typedef struct lef_xy lef_xy_t;

// Error codes emitted by the library.
enum lef_error {
	LEF_OK = 0,
	LEF_ERR_SYNTAX,
};

// Point in 2D sace.
struct lef_xy {
	double x,y;
};


// LEF file
ASIC_API lef_t *lef_new();
ASIC_API void lef_free(lef_t *lef);
ASIC_API size_t lef_get_num_macros(lef_t*);
ASIC_API lef_macro_t *lef_get_macro(lef_t*, size_t idx);
ASIC_API void lef_add_macro(lef_t*, lef_macro_t *macro);


// Macro
ASIC_API lef_macro_t *lef_new_macro(const char *name);
ASIC_API void lef_free_macro(lef_macro_t*);
ASIC_API const char *lef_macro_get_name(lef_macro_t*);
ASIC_API void lef_macro_set_size(lef_macro_t*, lef_xy_t);
ASIC_API void lef_macro_set_origin(lef_macro_t*, lef_xy_t);
ASIC_API lef_xy_t lef_macro_get_size(lef_macro_t*);
ASIC_API size_t lef_macro_get_num_pins(lef_macro_t*);
ASIC_API lef_pin_t *lef_macro_get_pin(lef_macro_t*, size_t idx);
ASIC_API void lef_macro_add_pin(lef_macro_t*, lef_pin_t*);


// Pin
ASIC_API struct lef_pin *lef_new_pin(const char *name);
ASIC_API void lef_free_pin(lef_pin_t*);
ASIC_API size_t lef_pin_get_num_ports(lef_pin_t*);
ASIC_API lef_port_t *lef_pin_get_port(lef_pin_t*, size_t idx);
ASIC_API const char *lef_pin_get_name(lef_pin_t*);
ASIC_API void lef_pin_add_port(lef_pin_t*, lef_port_t*);


// Port
ASIC_API lef_port_t *lef_new_port();
ASIC_API void lef_free_port(lef_port_t*);
ASIC_API enum lef_port_class lef_port_get_class(lef_port_t*);
ASIC_API size_t lef_port_get_num_geos(lef_port_t*);
ASIC_API lef_geo_t *lef_port_get_geo(lef_port_t*, size_t idx);
ASIC_API void lef_port_add_geometry(lef_port_t*, lef_geo_t*);


// Geometry
ASIC_API lef_geo_layer_t *lef_new_geo_layer(const char*);
ASIC_API void lef_free_geo_layer(lef_geo_layer_t*);
ASIC_API void lef_geo_layer_add_shape(lef_geo_layer_t*, lef_geo_shape_t*);
ASIC_API size_t lef_geo_layer_get_num_shapes(lef_geo_layer_t*);
ASIC_API lef_geo_shape_t *lef_geo_layer_get_shape(lef_geo_layer_t*, size_t idx);
ASIC_API const char *lef_geo_layer_get_name(lef_geo_layer_t*);


// Geometry Shape
ASIC_API lef_geo_shape_t *lef_new_geo_shape(int kind, unsigned num_points, lef_xy_t *points);
ASIC_API void lef_free_geo_shape(lef_geo_shape_t*);
ASIC_API unsigned lef_geo_shape_get_num_points(lef_geo_shape_t*);
ASIC_API lef_xy_t *lef_geo_shape_get_points(lef_geo_shape_t*);


// I/O
ASIC_API int lef_write(lef_t*, const char*);
ASIC_API int lef_read(lef_t**, const char*);



/** @} */
${HEADER_SUFFIX}
