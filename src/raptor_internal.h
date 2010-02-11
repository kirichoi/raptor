/* -*- Mode: c; c-basic-offset: 2 -*-
 *
 * raptor_internal.h - Redland Parser Toolkit for RDF (Raptor) internals
 *
 * Copyright (C) 2002-2009, David Beckett http://www.dajobe.org/
 * Copyright (C) 2002-2004, University of Bristol, UK http://www.bristol.ac.uk/
 * 
 * This package is Free Software and part of Redland http://librdf.org/
 * 
 * It is licensed under the following three licenses as alternatives:
 *   1. GNU Lesser General Public License (LGPL) V2.1 or any newer version
 *   2. GNU General Public License (GPL) V2 or any newer version
 *   3. Apache License, V2.0 or any newer version
 * 
 * You may not use this file except in compliance with at least one of
 * the above three licenses.
 * 
 * See LICENSE.html or LICENSE.txt at the top of this package for the
 * complete terms and further detail along with the license texts for
 * the licenses in COPYING.LIB, COPYING and LICENSE-2.0.txt respectively.
 * 
 * 
 */



#ifndef RAPTOR_INTERNAL_H
#define RAPTOR_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#define RAPTOR_EXTERN_C extern "C"
#else
#define RAPTOR_EXTERN_C
#endif

#ifdef RAPTOR_INTERNAL

/* for the memory allocation functions */
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#undef HAVE_STDLIB_H
#endif

#if defined(HAVE_DMALLOC_H) && defined(RAPTOR_MEMORY_DEBUG_DMALLOC)
#include <dmalloc.h>
#endif


/* Can be over-ridden or undefined in a config.h file or -Ddefine */
#ifndef RAPTOR_INLINE
#define RAPTOR_INLINE inline
#endif

#ifdef LIBRDF_DEBUG
#define RAPTOR_DEBUG 1
#endif

#if defined(RAPTOR_MEMORY_SIGN)
#define RAPTOR_SIGN_KEY 0x08A61080
void* raptor_sign_malloc(size_t size);
void* raptor_sign_calloc(size_t nmemb, size_t size);
void* raptor_sign_realloc(void *ptr, size_t size);
void raptor_sign_free(void *ptr);
  
#define RAPTOR_MALLOC(type, size)   raptor_sign_malloc(size)
#define RAPTOR_CALLOC(type, nmemb, size) raptor_sign_calloc(nmemb, size)
#define RAPTOR_REALLOC(type, ptr, size) raptor_sign_realloc(ptr, size)
#define RAPTOR_FREE(type, ptr)   raptor_sign_free(ptr)

#else
#define RAPTOR_MALLOC(type, size) malloc(size)
#define RAPTOR_CALLOC(type, nmemb, size) calloc(nmemb, size)
#define RAPTOR_REALLOC(type, ptr, size) realloc(ptr, size)
#define RAPTOR_FREE(type, ptr)   free((void*)ptr)

#endif

#ifdef RAPTOR_DEBUG
/* Debugging messages */
#define RAPTOR_DEBUG1(msg) do {fprintf(stderr, "%s:%d:%s: " msg, __FILE__, __LINE__, __func__); } while(0)
#define RAPTOR_DEBUG2(msg, arg1) do {fprintf(stderr, "%s:%d:%s: " msg, __FILE__, __LINE__, __func__, arg1);} while(0)
#define RAPTOR_DEBUG3(msg, arg1, arg2) do {fprintf(stderr, "%s:%d:%s: " msg, __FILE__, __LINE__, __func__, arg1, arg2);} while(0)
#define RAPTOR_DEBUG4(msg, arg1, arg2, arg3) do {fprintf(stderr, "%s:%d:%s: " msg, __FILE__, __LINE__, __func__, arg1, arg2, arg3);} while(0)
#define RAPTOR_DEBUG5(msg, arg1, arg2, arg3, arg4) do {fprintf(stderr, "%s:%d:%s: " msg, __FILE__, __LINE__, __func__, arg1, arg2, arg3, arg4);} while(0)
#define RAPTOR_DEBUG6(msg, arg1, arg2, arg3, arg4, arg5) do {fprintf(stderr, "%s:%d:%s: " msg, __FILE__, __LINE__, __func__, arg1, arg2, arg3, arg4, arg5);} while(0)

#if defined(HAVE_DMALLOC_H) && defined(RAPTOR_MEMORY_DEBUG_DMALLOC)
void* raptor_system_malloc(size_t size);
void raptor_system_free(void *ptr);
#define SYSTEM_MALLOC(size)   raptor_system_malloc(size)
#define SYSTEM_FREE(ptr)   raptor_system_free(ptr)
#else
#define SYSTEM_MALLOC(size)   malloc(size)
#define SYSTEM_FREE(ptr)   free(ptr)
#endif

#ifndef RAPTOR_ASSERT_DIE
#define RAPTOR_ASSERT_DIE abort();
#endif

#else
/* DEBUGGING TURNED OFF */

/* No debugging messages */
#define RAPTOR_DEBUG1(msg)
#define RAPTOR_DEBUG2(msg, arg1)
#define RAPTOR_DEBUG3(msg, arg1, arg2)
#define RAPTOR_DEBUG4(msg, arg1, arg2, arg3)
#define RAPTOR_DEBUG5(msg, arg1, arg2, arg3, arg4)
#define RAPTOR_DEBUG6(msg, arg1, arg2, arg3, arg4, arg5)

#define SYSTEM_MALLOC(size)   malloc(size)
#define SYSTEM_FREE(ptr)   free(ptr)

#ifndef RAPTOR_ASSERT_DIE
#define RAPTOR_ASSERT_DIE
#endif

#endif


#ifdef RAPTOR_DISABLE_ASSERT_MESSAGES
#define RAPTOR_ASSERT_REPORT(line)
#else
#define RAPTOR_ASSERT_REPORT(msg) fprintf(stderr, "%s:%d: (%s) assertion failed: " msg "\n", __FILE__, __LINE__, __func__);
#endif


#ifdef RAPTOR_DISABLE_ASSERT

#define RAPTOR_ASSERT(condition, msg) 
#define RAPTOR_ASSERT_RETURN(condition, msg, ret) 
#define RAPTOR_ASSERT_OBJECT_POINTER_RETURN(pointer, type) do { \
  if(!pointer) \
    return; \
} while(0)
#define RAPTOR_ASSERT_OBJECT_POINTER_RETURN_VALUE(pointer, type, ret)

#else

#define RAPTOR_ASSERT(condition, msg) do { \
  if(condition) { \
    RAPTOR_ASSERT_REPORT(msg) \
    RAPTOR_ASSERT_DIE \
  } \
} while(0)

#define RAPTOR_ASSERT_RETURN(condition, msg, ret) do { \
  if(condition) { \
    RAPTOR_ASSERT_REPORT(msg) \
    RAPTOR_ASSERT_DIE \
    return ret; \
  } \
} while(0)

#define RAPTOR_ASSERT_OBJECT_POINTER_RETURN(pointer, type) do { \
  if(!pointer) { \
    RAPTOR_ASSERT_REPORT("object pointer of type " #type " is NULL.") \
    RAPTOR_ASSERT_DIE \
    return; \
  } \
} while(0)

#define RAPTOR_ASSERT_OBJECT_POINTER_RETURN_VALUE(pointer, type, ret) do { \
  if(!pointer) { \
    RAPTOR_ASSERT_REPORT("object pointer of type " #type " is NULL.") \
    RAPTOR_ASSERT_DIE \
    return ret; \
  } \
} while(0)

#endif


/* Fatal errors - always happen */
#define RAPTOR_FATAL1(msg) do {fprintf(stderr, "%s:%d:%s: fatal error: " msg, __FILE__, __LINE__ , __func__); abort();} while(0)
#define RAPTOR_FATAL2(msg,arg) do {fprintf(stderr, "%s:%d:%s: fatal error: " msg, __FILE__, __LINE__ , __func__, arg); abort();} while(0)
#define RAPTOR_FATAL3(msg,arg1,arg2) do {fprintf(stderr, "%s:%d:%s: fatal error: " msg, __FILE__, __LINE__ , __func__, arg1, arg2); abort();} while(0)

#define MAX_ASCII_INT_SIZE 13
  
/* XML parser includes */
#ifdef RAPTOR_XML_EXPAT
#ifdef HAVE_EXPAT_H
#include <expat.h>
#endif
#ifdef HAVE_XMLPARSE_H
#include <xmlparse.h>
#endif
#endif




#ifdef RAPTOR_XML_LIBXML

#include <libxml/parser.h>


/* libxml-only prototypes */


/* raptor_libxml.c exports */
extern void raptor_libxml_sax_init(raptor_sax2* sax2);
extern void raptor_libxml_generic_error(void* user_data, const char *msg, ...) RAPTOR_PRINTF_FORMAT(2, 3);

extern int raptor_libxml_init(raptor_world* world);
extern void raptor_libxml_finish(raptor_world* world);

extern void raptor_libxml_validation_error(void *context, const char *msg, ...) RAPTOR_PRINTF_FORMAT(2, 3);
extern void raptor_libxml_validation_warning(void *context, const char *msg, ...) RAPTOR_PRINTF_FORMAT(2, 3);
void raptor_libxml_free(xmlParserCtxtPtr xc);

/* raptor_parse.c - exported to libxml part */
extern void raptor_libxml_update_document_locator(raptor_sax2* sax2, raptor_locator* locator);

/* end of libxml-only */
#endif


/* expat-only prototypes */

#ifdef RAPTOR_XML_EXPAT
/* raptor_expat.c exports */
extern void raptor_expat_init(raptor_sax2* sax2, raptor_uri *base_uri);
extern void raptor_expat_update_document_locator(raptor_sax2* sax2, raptor_locator *locator);

/* raptor_parse.c */
void raptor_xml_unparsed_entity_decl_handler(void *user_data, const unsigned char* entityName, const unsigned char* base, const unsigned char* systemId, const unsigned char* publicId, const unsigned char* notationName);
int raptor_xml_external_entity_ref_handler(void *user_data, const unsigned char* context, const unsigned char* base, const unsigned char* systemId, const unsigned char* publicId);

/* end of expat-only */
#endif


typedef struct raptor_parser_factory_s raptor_parser_factory;
typedef struct raptor_serializer_factory_s raptor_serializer_factory;
typedef struct raptor_id_set_s raptor_id_set;
typedef struct raptor_uri_detail_s raptor_uri_detail;

/* raptor_concepts.c */

/*
 * raptor_rdf_ns_term_id:
 *
 * RDF namespace syntax terms, properties and classes. 
 *
 * The order must match names in the raptor_rdf_ns_terms_info table
 *
 */
typedef enum {
  /* These terms are used only in the RDF/XML syntax; never in RDF graph */
  RDF_NS_RDF             = 0,
  RDF_NS_Description     = 1,
  RDF_NS_li              = 2,
  RDF_NS_about           = 3,
  RDF_NS_aboutEach       = 4,
  RDF_NS_aboutEachPrefix = 5,
  RDF_NS_ID              = 6,
  RDF_NS_bagID           = 7,
  RDF_NS_resource        = 8,
  RDF_NS_parseType       = 9,
  RDF_NS_nodeID          = 10,
  RDF_NS_datatype        = 11,
  /* These terms are all properties in RDF model (of type rdf:Property) */
  RDF_NS_type            = 12,
  RDF_NS_value           = 13,
  RDF_NS_subject         = 14,
  RDF_NS_predicate       = 15,
  RDF_NS_object          = 16,
  RDF_NS_first           = 17,
  RDF_NS_rest            = 18,
  /* These terms are all classes in the RDF model (of type rdfs:Class) */
  RDF_NS_Seq             = 19,
  RDF_NS_Bag             = 20,
  RDF_NS_Alt             = 21,
  RDF_NS_Statement       = 22,
  RDF_NS_Property        = 23,
  RDF_NS_List            = 24,
  /* These terms are all resources in the RDF model (of type rdfs:Resource) */
  RDF_NS_nil             = 25,

  /* These terms are datatypes (used as a literal datatype URI) */
  RDF_NS_XMLLiteral      = 26,
  RDF_NS_PlainLiteral    = 27, /* http://www.w3.org/TR/rdf-text/ */

  /* These terms are internal */
  RDF_NS_LAST_SYNTAX_TERM = RDF_NS_datatype,

  RDF_NS_LAST            = RDF_NS_PlainLiteral
} raptor_rdf_ns_term_id;


typedef struct { 
  /* term name */
  const char *name;

  /* RDF/XML: the statement object type of this when used as an attribute */
  raptor_term_type type;

  /* RDF/XML: name restrictions */
  unsigned int allowed_as_nodeElement : 1;
  unsigned int allowed_as_propertyElement : 1;
  unsigned int allowed_as_propertyAttribute : 1;
  unsigned int allowed_unprefixed_on_attribute : 1;
} raptor_rdf_ns_term_info;


extern const raptor_rdf_ns_term_info raptor_rdf_ns_terms_info[(RDF_NS_LAST + 1) + 1];

#define RAPTOR_RDF_RDF_URI(world)         world->concepts[RDF_NS_RDF]
#define RAPTOR_RDF_Description_URI(world) world->concepts[RDF_NS_Description]
#define RAPTOR_RDF_li_URI(world)          world->concepts[RDF_NS_li]
#define RAPTOR_RDF_about(world)           world->concepts[RDF_NS_about]
#define RAPTOR_RDF_aboutEach(world)       world->concepts[RDF_NS_aboutEach]
#define RAPTOR_RDF_aboutEachPrefix(world) world->concepts[RDF_NS_aboutEachPrefix]
#define RAPTOR_RDF_ID_URI(world)              world->concepts[RDF_NS_ID]
#define RAPTOR_RDF_bagID_URI(world)           world->concepts[RDF_NS_bagID]
#define RAPTOR_RDF_resource_URI(world)        world->concepts[RDF_NS_resource]
#define RAPTOR_RDF_parseType_URI(world)       world->concepts[RDF_NS_parseType]
#define RAPTOR_RDF_nodeID_URI(world)          world->concepts[RDF_NS_nodeID]
#define RAPTOR_RDF_datatype_URI(world)        world->concepts[RDF_NS_datatype]

#define RAPTOR_RDF_type_URI(world)        world->concepts[RDF_NS_type]
#define RAPTOR_RDF_value_URI(world)       world->concepts[RDF_NS_value]
#define RAPTOR_RDF_subject_URI(world)     world->concepts[RDF_NS_subject]
#define RAPTOR_RDF_predicate_URI(world)   world->concepts[RDF_NS_predicate]
#define RAPTOR_RDF_object_URI(world)      world->concepts[RDF_NS_object]
#define RAPTOR_RDF_first_URI(world)       world->concepts[RDF_NS_first]
#define RAPTOR_RDF_rest_URI(world)        world->concepts[RDF_NS_rest]
                                         
#define RAPTOR_RDF_Seq_URI(world)         world->concepts[RDF_NS_Seq]
#define RAPTOR_RDF_Bag_URI(world)         world->concepts[RDF_NS_Bag]
#define RAPTOR_RDF_Alt_URI(world)         world->concepts[RDF_NS_Alt]
#define RAPTOR_RDF_Statement_URI(world)   world->concepts[RDF_NS_Statement]
#define RAPTOR_RDF_Property_URI(world)    world->concepts[RDF_NS_Property]
#define RAPTOR_RDF_List_URI(world)        world->concepts[RDF_NS_List]

#define RAPTOR_RDF_nil_URI(world)          world->concepts[RDF_NS_nil]
#define RAPTOR_RDF_XMLLiteral_URI(world)   world->concepts[RDF_NS_XMLLiteral]
#define RAPTOR_RDF_PlainLiteral_URI(world) world->concepts[RDF_NS_PlainLiteral]


/* syntax only (RDF:RDF ... RDF:datatype) are not provided as terms */

#define RAPTOR_RDF_type_term(world)        world->terms[RDF_NS_type]
#define RAPTOR_RDF_value_term(world)       world->terms[RDF_NS_value]
#define RAPTOR_RDF_subject_term(world)     world->terms[RDF_NS_subject]
#define RAPTOR_RDF_predicate_term(world)   world->terms[RDF_NS_predicate]
#define RAPTOR_RDF_object_term(world)      world->terms[RDF_NS_object]
#define RAPTOR_RDF_first_term(world)       world->terms[RDF_NS_first]
#define RAPTOR_RDF_rest_term(world)        world->terms[RDF_NS_rest]
                                         
#define RAPTOR_RDF_Seq_term(world)         world->terms[RDF_NS_Seq]
#define RAPTOR_RDF_Bag_term(world)         world->terms[RDF_NS_Bag]
#define RAPTOR_RDF_Alt_term(world)         world->terms[RDF_NS_Alt]
#define RAPTOR_RDF_Statement_term(world)   world->terms[RDF_NS_Statement]
#define RAPTOR_RDF_Property_term(world)    world->terms[RDF_NS_Property]
#define RAPTOR_RDF_List_term(world)        world->terms[RDF_NS_List]

#define RAPTOR_RDF_nil_term(world)          world->terms[RDF_NS_nil]
#define RAPTOR_RDF_XMLLiteral_term(world)   world->terms[RDF_NS_XMLLiteral]
#define RAPTOR_RDF_PlainLiteral_term(world) world->terms[RDF_NS_PlainLiteral]


int raptor_concepts_init(raptor_world* world);
void raptor_concepts_finish(raptor_world* world);



/* raptor_iostream.c */
raptor_world* raptor_iostream_get_world(raptor_iostream *iostr);


/* Raptor Namespace Stack node */
struct raptor_namespace_stack_s {
  raptor_world* world;
  int size;

  int table_size;
  raptor_namespace** table;
  raptor_namespace* def_namespace;

  raptor_uri *rdf_ms_uri;
  raptor_uri *rdf_schema_uri;
};


/* Forms:
 * 1) prefix=NULL uri=<URI>      - default namespace defined
 * 2) prefix=NULL, uri=NULL      - no default namespace
 * 3) prefix=<prefix>, uri=<URI> - regular pair defined <prefix>:<URI>
 */
struct raptor_namespace_s {
  /* next down the stack, NULL at bottom */
  struct raptor_namespace_s* next;

  raptor_namespace_stack *nstack;

  /* NULL means is the default namespace */
  const unsigned char *prefix;
  /* needed to safely compare prefixed-names */
  int prefix_length;
  /* URI of namespace or NULL for default */
  raptor_uri *uri;
  /* parsing depth that this ns was added.  It will
   * be deleted when the parser leaves this depth 
   */
  int depth;
  /* Non 0 if is xml: prefixed name */
  int is_xml;
  /* Non 0 if is RDF M&S Namespace */
  int is_rdf_ms;
  /* Non 0 if is RDF Schema Namespace */
  int is_rdf_schema;
};

raptor_namespace** raptor_namespace_stack_to_array(raptor_namespace_stack *nstack, size_t *size_p);

#ifdef RAPTOR_XML_LIBXML
#define RAPTOR_LIBXML_MAGIC 0x8AF108
#endif


/*
 * Raptor parser object
 */
struct raptor_parser_s {
  raptor_world* world;

#ifdef RAPTOR_XML_LIBXML
  int magic;
#endif

  /* can be filled with error location information */
  raptor_locator locator;

  /* non 0 if parser had fatal error and cannot continue */
  int failed;

  /* generated ID counter */
  int genid;

  /* base URI of RDF/XML */
  raptor_uri *base_uri;

  /* static statement for use in passing to user code */
  raptor_statement statement;

  /* Options */
  int options[RAPTOR_OPTION_LAST+1];

  /* stuff for our user */
  void *user_data;

  /* parser callbacks */
  raptor_statement_handler statement_handler;

  raptor_graph_handler graph_handler;

  void *generate_id_handler_user_data;
  raptor_generate_id_handler generate_id_handler;

  int default_generate_id_handler_base;
  char *default_generate_id_handler_prefix;
  size_t default_generate_id_handler_prefix_length;

  void* uri_filter_user_data;
  raptor_uri_filter_func uri_filter;

  /* parser specific stuff */
  void *context;

  struct raptor_parser_factory_s* factory;

  /* namespace callback */
  raptor_namespace_handler namespace_handler;

  void* namespace_handler_user_data;

  raptor_stringbuffer* sb;

  /* raptor_www pointer stored here to allow cleanup on error */
  raptor_www* www;

  /* internal data for lexers */
  void* lexer_user_data;

  /* OPTION:
   * HTTP Cache-Control: header value to send (default NULL)
   * RAPTOR_OPTION_WWW_HTTP_CACHE_CONTROL
   */
  const char* cache_control;

  /* OPTION:
   * HTTP User-Agent: header value to send (default NULL)
   * RAPTOR_OPTION_WWW_HTTP_USER_AGENT
   */
  const char* user_agent;

  /* NOTE: if anything a user can set is added here check that
   * raptor_parser_copy_user_state() is updated to copy it.
   */
};


/** A list of (MIME Type, Q) values */
struct raptor_type_q_s {
  const char* mime_type;
  size_t mime_type_len;
  int q; /* 0-10 standing for 0.0-1.0 */
};
typedef struct raptor_type_q_s raptor_type_q;


/** A Parser Factory for a syntax */
struct raptor_parser_factory_s {
  raptor_world* world;

  struct raptor_parser_factory_s* next;

  /* syntax name */
  const char* name;
  /* alternate syntax name; not mentioned in enumerations */
  const char* alias;

  /* syntax readable label */
  const char* label;

  /* syntax MIME type (or NULL) */
  raptor_sequence* mime_types;

  /* syntax URI (or NULL) */
  const unsigned char* uri_string;
  
  /* the rest of this structure is populated by the
     parser-specific register function */
  size_t context_length;
  
  /* create a new parser */
  int (*init)(raptor_parser* parser, const char *name);
  
  /* destroy a parser */
  void (*terminate)(raptor_parser* parser);
  
  /* start a parse */
  int (*start)(raptor_parser* parser);
  
  /* parse a chunk of memory */
  int (*chunk)(raptor_parser* parser, const unsigned char *buffer, size_t len, int is_end);

  /* finish the parser factory */
  void (*finish_factory)(raptor_parser_factory* factory);

  /* score recognition of the syntax by a block of characters, the
   *  content identifier or it's suffix or a mime type
   *  (different from the factory-registered one)
   */
  int (*recognise_syntax)(raptor_parser_factory* factory, const unsigned char *buffer, size_t len, const unsigned char *identifier, const unsigned char *suffix, const char *mime_type);

  /* get the Content-Type value of a URI request */
  void (*content_type_handler)(raptor_parser* rdf_parser, const char* content_type);

  /* get the Accept header of a URI request (OPTIONAL) */
  const char* (*accept_header)(raptor_parser* rdf_parser);

  /* non-0 if this parser needs a base URI */
  int need_base_uri;

  /* get the current generated ID base (OPTIONAL) */
  int (*get_current_base_id)(raptor_parser* rdf_parser);

  /* get the name (OPTIONAL) */
  const char* (*get_name)(raptor_parser* rdf_parser);
};


/*
 * Raptor serializer object
 */
struct raptor_serializer_s {
  raptor_world* world;

  /* can be filled with error location information */
  raptor_locator locator;
  
  /* OPTION:
   * non 0 to write base URI to document header (@base)
   */
  int option_write_base_uri;

  /* OPTION:
   * non 0 to write relative URIs wherever possible
   */
  int option_relative_uris;

  /* OPTIONS:
   * non NULL to override default border color
   */
  unsigned char *option_resource_border;
  unsigned char *option_literal_border;
  unsigned char *option_bnode_border;

  /* OPTIONS:
   * non NULL to fill with value
   */
  unsigned char *option_resource_fill;
  unsigned char *option_literal_fill;
  unsigned char *option_bnode_fill;

  void *error_user_data;
  void *warning_user_data;

  raptor_message_handler error_handler;
  raptor_message_handler warning_handler;

  /* non 0 if serializer had fatal error and cannot continue */
  int failed;

  /* base URI of RDF/XML */
  raptor_uri *base_uri;

  /* serializer specific stuff */
  void *context;

  /* destination stream for the serialization */
  raptor_iostream *iostream;

  /* if true, iostream was made here so free it */
  int free_iostream_on_end;
  
  struct raptor_serializer_factory_s* factory;

  /* XML 1.0 (10) or XML 1.1 (11) */
  int xml_version;

  /* OPTION:
   * non 0 to write XML 1.0 or 1.1 declaration (default 1)
   */
  int option_write_xml_declaration;

  /* OPTION:
   * JSON serializer callback function name
   */
  unsigned char *option_json_callback;

  /* OPTION:
   * JSON serializer extra data
   */
  unsigned char *option_json_extra_data;

  /* OPTION:
   * RSS serializer triples
   */
  unsigned char *option_rss_triples;

  /* OPTION:
   * Atom serializer entry URI string
   */
  unsigned char *option_atom_entry_uri;

  /* OPTION:
   * Namespace-prefix elements OR prefer unprefixed elements.
   */
  int option_prefix_elements;
};


/** A Serializer Factory for a syntax */
struct raptor_serializer_factory_s {
  raptor_world* world;

  struct raptor_serializer_factory_s* next;

  /* syntax name */
  const char* name;
  /* alternate syntax name; not mentioned in enumerations */
  const char* alias;

  /* syntax readable label */
  const char* label;
  /* syntax MIME type (or NULL) */
  const char* mime_type;
  /* syntax URI (or NULL) */
  const unsigned char* uri_string;
  
  /* the rest of this structure is populated by the
     serializer-specific register function */
  size_t context_length;
  
  /* create a new serializer */
  int (*init)(raptor_serializer* serializer, const char *name);
  
  /* destroy a serializer */
  void (*terminate)(raptor_serializer* serializer);
  
  /* add a namespace */
  int (*declare_namespace)(raptor_serializer* serializer, raptor_uri *uri, const unsigned char *prefix);
  
  /* start a serialization */
  int (*serialize_start)(raptor_serializer* serializer);
  
  /* serialize a statement */
  int (*serialize_statement)(raptor_serializer* serializer, raptor_statement *statment);

  /* end a serialization */
  int (*serialize_end)(raptor_serializer* serializer);
  
  /* finish the serializer factory */
  void (*finish_factory)(raptor_serializer_factory* factory);

  /* add a namespace using an existing namespace */
  int (*declare_namespace_from_namespace)(raptor_serializer* serializer, raptor_namespace *nspace);
};


/* for raptor_parse_uri_write_bytes() when used as a handler for
 * raptor_www_set_write_bytes_handler()
 */
typedef struct 
{
  raptor_parser* rdf_parser;
  raptor_uri* base_uri;
  raptor_uri* final_uri;
  int started;
} raptor_parse_bytes_context;


/* raptor_serialize.c */
int raptor_serializer_register_factory(raptor_world* world, const char *name, const char *label, const char *mime_type, const char *alias, const unsigned char *uri_string, int (*factory) (raptor_serializer_factory*));


/* raptor_general.c */

raptor_parser_factory* raptor_parser_register_factory(raptor_world* world, const char *name, const char *label, int (*factory) (raptor_parser_factory*));
int raptor_parser_factory_add_alias(raptor_parser_factory* factory, const char *alias);
int raptor_parser_factory_add_mime_type(raptor_parser_factory* factory, const char* mime_type, int q);
int raptor_parser_factory_add_uri(raptor_parser_factory* factory, const unsigned char *uri_string);

unsigned char* raptor_parser_internal_generate_id(raptor_parser *rdf_parser, raptor_genid_type type, unsigned char *user_bnodeid);

#ifdef RAPTOR_DEBUG
void raptor_stats_print(raptor_parser *rdf_parser, FILE *stream);
#endif
const char* raptor_basename(const char *name);
raptor_statement* raptor_new_statement(raptor_world *world);
raptor_statement* raptor_new_statement_from_nodes(raptor_world* world, raptor_term *subject, raptor_term *predicate, raptor_term *object);
raptor_statement* raptor_statement_copy(raptor_statement *statement);
raptor_term* raptor_term_copy(raptor_term* term);
int raptor_term_compare(const raptor_term *t1, const raptor_term *t2);
int raptor_term_equals(raptor_term* term1, raptor_term* term2);
int raptor_term_print_as_ntriples(FILE* stream, const raptor_term *term);

/* raptor_parse.c */
raptor_parser_factory* raptor_world_get_parser_factory(raptor_world* world, const char *name);  
void raptor_delete_parser_factories(void);
const char* raptor_parser_get_accept_header_all(raptor_world* world);
int raptor_parser_set_uri_filter_no_net(void *user_data, raptor_uri* uri);
void raptor_parse_uri_write_bytes(raptor_www* www, void *userdata, const void *ptr, size_t size, size_t nmemb);
void raptor_parser_fatal_error(raptor_parser* parser, const char *message, ...) RAPTOR_PRINTF_FORMAT(2, 3);
void raptor_parser_error(raptor_parser* parser, const char *message, ...) RAPTOR_PRINTF_FORMAT(2, 3);
void raptor_parser_simple_error(void* parser, const char *message, ...) RAPTOR_PRINTF_FORMAT(2, 3);
void raptor_parser_error_varargs(raptor_parser* parser, const char *message, va_list arguments) RAPTOR_PRINTF_FORMAT(2, 0);
void raptor_parser_warning(raptor_parser* parser, const char *message, ...) RAPTOR_PRINTF_FORMAT(2, 3);

/* logging */
#define RAPTOR_ERROR_HANDLER_MAGIC 0xD00DB1FF

void raptor_world_internal_set_ignore_errors(raptor_world* world, int flag);
void raptor_log_error_varargs(raptor_world* world, raptor_log_level level, raptor_locator* locator, const char* message, va_list arguments) RAPTOR_PRINTF_FORMAT(4, 0);
void raptor_log_error_formatted(raptor_world* world, raptor_log_level level, raptor_locator* locator, const char* message, ...) RAPTOR_PRINTF_FORMAT(4, 5);
void raptor_log_error(raptor_world* world, raptor_log_level level, raptor_locator* locator, const char* message);


/* raptor_parse.c */

typedef struct raptor_rdfxml_parser_s raptor_rdfxml_parser;

/* Prototypes for common expat/libxml parsing event-handling functions */
extern void raptor_xml_start_element_handler(void *user_data, const unsigned char *name, const unsigned char **atts);
extern void raptor_xml_end_element_handler(void *user_data, const unsigned char *name);
/* s is not 0 terminated. */
extern void raptor_xml_characters_handler(void *user_data, const unsigned char *s, int len);
extern void raptor_xml_cdata_handler(void *user_data, const unsigned char *s, int len);
void raptor_xml_comment_handler(void *user_data, const unsigned char *s);

#if RAPTOR_DEBUG > 1
void raptor_rdfxml_parser_stats_print(raptor_rdfxml_parser* rdf_xml_parser, FILE *stream);
#endif

int raptor_parser_copy_user_state(raptor_parser *to_parser, raptor_parser *from_parser);

/* raptor_option.c */

/* These are bits and may be bit-ORed */
/**
 * raptor_option_area:
 * @RAPTOR_OPTION_AREA_NONE: internal
 * @RAPTOR_OPTION_AREA_PARSER: #raptor_parser (public)
 * @RAPTOR_OPTION_AREA_SERIALIZER: #raptor_serializer (public)
 * @RAPTOR_OPTION_AREA_XML_WRITER: #raptor_xml_writer (public)
 * @RAPTOR_OPTION_AREA_TURTLE_WRITER: #raptor_turtle_writer (internal)
 * @RAPTOR_OPTION_AREA_SAX2: #raptor_sax2 (public)
 *
 * Internal - raptor option areas
*/
typedef enum {
 RAPTOR_OPTION_AREA_NONE = 0,
 RAPTOR_OPTION_AREA_PARSER = 1,
 RAPTOR_OPTION_AREA_SERIALIZER = 2,
 RAPTOR_OPTION_AREA_XML_WRITER = 4,
 RAPTOR_OPTION_AREA_TURTLE_WRITER = 8,
 RAPTOR_OPTION_AREA_SAX2 = 16
} raptor_option_area;

int raptor_option_value_is_numeric(const raptor_option option);
int raptor_option_is_valid_for_area(const raptor_option option, raptor_option_area area);

/* raptor_general.c */
extern int raptor_valid_xml_ID(raptor_parser *rdf_parser, const unsigned char *string);
int raptor_check_ordinal(const unsigned char *name);

/* raptor_locator.c */


#ifdef HAVE_STRCASECMP
#define raptor_strcasecmp strcasecmp
#define raptor_strncasecmp strncasecmp
#else
#ifdef HAVE_STRICMP
#define raptor_strcasecmp stricmp
#define raptor_strncasecmp strnicmp
#endif
#endif


/* raptor_nfc.c */
int raptor_nfc_check (const unsigned char* string, size_t len, int *error);


/* raptor_namespace.c */

#ifdef RAPTOR_DEBUG
void raptor_namespace_print(FILE *stream, raptor_namespace* ns);
#endif

void raptor_parser_start_namespace(raptor_parser* rdf_parser, raptor_namespace* nspace);


/* 
 * Raptor XML-namespace qualified name (qname), for elements or attributes 
 *
 * namespace is only defined when the XML name has a namespace and
 * only then is uri also given.
 */
struct raptor_qname_s {
  raptor_world* world;
  /* Name - always present */
  const unsigned char *local_name;
  int local_name_length;
  /* Namespace or NULL if not in a namespace */
  const raptor_namespace *nspace;
  /* URI of namespace+local_name or NULL if not defined */
  raptor_uri *uri;
  /* optional value - used when name is an attribute */
  const unsigned char *value;
  unsigned int value_length;
};



/* raptor_qname.c */
#ifdef RAPTOR_DEBUG
void raptor_qname_print(FILE *stream, raptor_qname* name);
#endif


/* raptor_uri.c */

int raptor_uri_init(raptor_world* world);
void raptor_uri_finish(raptor_world* world);
raptor_uri* raptor_new_uri_from_rdf_ordinal(raptor_world* world, int ordinal);

/* parsers */
int raptor_init_parser_rdfxml(raptor_world* world);
int raptor_init_parser_ntriples(raptor_world* world);
int raptor_init_parser_turtle(raptor_world* world);
int raptor_init_parser_trig(raptor_world* world);
int raptor_init_parser_n3(raptor_world* world);
int raptor_init_parser_grddl_common(raptor_world* world);
int raptor_init_parser_grddl(raptor_world* world);
int raptor_init_parser_guess(raptor_world* world);
int raptor_init_parser_rss(raptor_world* world);
int raptor_init_parser_rdfa(raptor_world* world);

void raptor_terminate_parser_grddl_common(raptor_world *world);

/* raptor_parse.c */
int raptor_parsers_init(raptor_world* world);
void raptor_parsers_finish(raptor_world *world);

void raptor_parser_save_content(raptor_parser* rdf_parser, int save);
const unsigned char* raptor_parser_get_content(raptor_parser* rdf_parser, size_t* length_p);
void raptor_parser_set_graph_name(raptor_parser* parser, raptor_uri* uri);
int raptor_parser_get_current_base_id(raptor_parser* parser);

/* raptor_rss.c */
int raptor_init_serializer_rss10(raptor_world* world);
int raptor_init_serializer_atom(raptor_world* world);

extern const unsigned char * const raptor_atom_namespace_uri;

/* raptor_rfc2396.c */
raptor_uri_detail* raptor_new_uri_detail(const unsigned char *uri_string);
void raptor_free_uri_detail(raptor_uri_detail* uri_detail);
unsigned char* raptor_uri_detail_to_string(raptor_uri_detail *ud, size_t* len_p);

/* serializers */
int raptor_init_serializer_rdfxml(raptor_world* world);
int raptor_init_serializer_ntriples(raptor_world* world);
int raptor_init_serializer_dot(raptor_world* world);
int raptor_init_serializer_simple(raptor_world* world);

/* raptor_serializer.c */
int raptor_serializers_init(raptor_world* world);
void raptor_serializers_finish(raptor_world* world);

/* raptor_serialize_rdfxmla.c */  
int raptor_init_serializer_rdfxmla(raptor_world* world);

/* raptor_serialize_turtle.c */  
int raptor_init_serializer_turtle(raptor_world* world);

/* raptor_serialize_json.c */  
int raptor_init_serializer_json(raptor_world* world);

/* raptor_utf8.c */
int raptor_unicode_is_namestartchar(raptor_unichar c);
int raptor_unicode_is_namechar(raptor_unichar c);
int raptor_utf8_is_nfc(const unsigned char *input, size_t length);

/* raptor_www*.c */
#ifdef RAPTOR_WWW_LIBXML
#include <libxml/parser.h>
#include <libxml/xmlerror.h>
#include <libxml/nanohttp.h>
#endif

#ifdef RAPTOR_WWW_LIBCURL
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#endif

/* Size of buffer used in various raptor_www places for I/O  */
#ifndef RAPTOR_WWW_BUFFER_SIZE
#define RAPTOR_WWW_BUFFER_SIZE 4096
#endif

/* WWW library state */
struct  raptor_www_s {
  raptor_world* world;
  char *type;
  int free_type;
  int total_bytes;
  int failed;
  int status_code;

  raptor_uri *uri;

#ifdef RAPTOR_WWW_LIBCURL
  CURL* curl_handle;
  char error_buffer[CURL_ERROR_SIZE];
  int curl_init_here;
  int checked_status;
#endif

#ifdef RAPTOR_WWW_LIBXML
  void *ctxt;
  char buffer[RAPTOR_WWW_BUFFER_SIZE];
  int is_end;
  void *old_xmlGenericErrorContext;
#endif

#ifdef RAPTOR_WWW_LIBFETCH
  char buffer[RAPTOR_WWW_BUFFER_SIZE];
#endif

  char *user_agent;

  /* proxy URL string or NULL for none */
  char *proxy;
  
  void *write_bytes_userdata;
  raptor_www_write_bytes_handler write_bytes;
  void *content_type_userdata;
  raptor_www_content_type_handler content_type;

  void* uri_filter_user_data;
  raptor_uri_filter_func uri_filter;

  /* can be filled with error location information */
  raptor_locator locator;

  char *http_accept;

  FILE* handle;

  int connection_timeout;

  /* The URI returned after any redirections */
  raptor_uri* final_uri;

  void *final_uri_userdata;
  raptor_www_final_uri_handler final_uri_handler;

  char* cache_control;
};



/* internal */
void raptor_www_libxml_init(raptor_www *www);
void raptor_www_libxml_free(raptor_www *www);
int raptor_www_libxml_fetch(raptor_www *www);

void raptor_www_error(raptor_www *www, const char *message, ...) RAPTOR_PRINTF_FORMAT(2, 3);

void raptor_www_curl_init(raptor_www *www);
void raptor_www_curl_free(raptor_www *www);
int raptor_www_curl_fetch(raptor_www *www);

void raptor_www_libfetch_init(raptor_www *www);
void raptor_www_libfetch_free(raptor_www *www);
int raptor_www_libfetch_fetch(raptor_www *www);

/* raptor_set.c */
raptor_id_set* raptor_new_id_set(raptor_world* world);
void raptor_free_id_set(raptor_id_set* set);
int raptor_id_set_add(raptor_id_set* set, raptor_uri* base_uri, const unsigned char *item, size_t item_len);
#if RAPTOR_DEBUG > 1
void raptor_id_set_stats_print(raptor_id_set* set, FILE *stream);
#endif

/* raptor_sax2.c */
/*
 * SAX2 elements/attributes on stack 
 */
struct raptor_xml_element_s {
  /* NULL at bottom of stack */
  struct raptor_xml_element_s *parent;
  raptor_qname *name;
  raptor_qname **attributes;
  unsigned int attribute_count;

  /* value of xml:lang attribute on this element or NULL */
  const unsigned char *xml_language;

  /* URI of xml:base attribute value on this element or NULL */
  raptor_uri *base_uri;

  /* CDATA content of element and checks for mixed content */
  raptor_stringbuffer* content_cdata_sb;
  unsigned int content_cdata_length;
  /* how many cdata blocks seen */
  unsigned int content_cdata_seen;
  /* how many contained elements seen */
  unsigned int content_element_seen;

  raptor_sequence *declared_nspaces;

  void* user_data;
};


struct raptor_sax2_s {
#ifdef RAPTOR_XML_LIBXML
  int magic;
#endif
  raptor_world* world;
  void* user_data;
  
#ifdef RAPTOR_XML_EXPAT
  XML_Parser xp;
#ifdef EXPAT_UTF8_BOM_CRASH
  int tokens_count; /* used to see if trying to get location info is safe */
#endif
#endif
#ifdef RAPTOR_XML_LIBXML
  /* structure holding sax event handlers */
  xmlSAXHandler sax;
  /* parser context */
  xmlParserCtxtPtr xc;
  /* pointer to SAX document locator */
  xmlSAXLocatorPtr loc;

#if LIBXML_VERSION < 20425
  /* flag for some libxml eversions*/
  int first_read;
#endif

#endif  

  /* element depth */
  int depth;

  /* stack of elements - elements add after current_element */
  raptor_xml_element *root_element;
  raptor_xml_element *current_element;

  /* start of an element */
  raptor_sax2_start_element_handler start_element_handler;
  /* end of an element */
  raptor_sax2_end_element_handler end_element_handler;
  /* characters */
  raptor_sax2_characters_handler characters_handler;
  /* like <![CDATA[...]> */
  raptor_sax2_cdata_handler cdata_handler;
  /* comment */
  raptor_sax2_comment_handler comment_handler;
  /* unparsed (NDATA) entity */
  raptor_sax2_unparsed_entity_decl_handler unparsed_entity_decl_handler;
  /* external entity reference */
  raptor_sax2_external_entity_ref_handler external_entity_ref_handler;

  raptor_locator *locator;

  /* New XML namespace callback */
  raptor_namespace_handler  namespace_handler;

  /* OPTION: 
   * non 0 if require normalizing xml:lang attribute values to lowercase.
   */
  int option_normalize_language;

  /* OPTION: 
   * non 0 if network access is denied
   */
  int option_no_net;

  /* stack of namespaces, most recently added at top */
  raptor_namespace_stack namespaces; /* static */

  /* base URI for resolving relative URIs or xml:base URIs */
  raptor_uri* base_uri;

  /* sax2 init failed - do not try to do anything with it */
  int failed;
};

int raptor_sax2_init(raptor_world* world);
void raptor_sax2_finish(raptor_world* world);


raptor_xml_element* raptor_xml_element_pop(raptor_sax2* sax2);
void raptor_xml_element_push(raptor_sax2* sax2, raptor_xml_element* element);
int raptor_sax2_get_depth(raptor_sax2* sax2);
void raptor_sax2_inc_depth(raptor_sax2* sax2);
void raptor_sax2_dec_depth(raptor_sax2* sax2);
void raptor_sax2_update_document_locator(raptor_sax2* sax2, raptor_locator* locator);
int raptor_sax2_set_option(raptor_sax2* sax2, raptor_option option, int value);
  
#ifdef RAPTOR_DEBUG
void raptor_print_xml_element(raptor_xml_element *element, FILE* stream);
#endif

void raptor_sax2_start_element(void* user_data, const unsigned char *name, const unsigned char **atts);
void raptor_sax2_end_element(void* user_data, const unsigned char *name);
void raptor_sax2_characters(void* user_data, const unsigned char *s, int len);
void raptor_sax2_cdata(void* user_data, const unsigned char *s, int len);
void raptor_sax2_comment(void* user_data, const unsigned char *s);
void raptor_sax2_unparsed_entity_decl(void* user_data, const unsigned char* entityName, const unsigned char* base, const unsigned char* systemId, const unsigned char* publicId, const unsigned char* notationName);
int raptor_sax2_external_entity_ref(void* user_data, const unsigned char* context, const unsigned char* base, const unsigned char* systemId, const unsigned char* publicId);


/* turtle_parser.y and turtle_lexer.l */
typedef struct raptor_turtle_parser_s raptor_turtle_parser;

/* n3_parser.y and n3_lexer.l */
typedef struct raptor_n3_parser_s raptor_n3_parser;

/* raptor_rfc2396.c */
struct raptor_uri_detail_s
{
  size_t uri_len;
  /* buffer is the same size as the original uri_len */
  unsigned char *buffer;

  /* URI Components.  These all point into buffer */
  unsigned char *scheme;
  unsigned char *authority;
  unsigned char *path;
  unsigned char *query;
  unsigned char *fragment;

  /* Lengths of the URI Components  */
  size_t scheme_len;
  size_t authority_len;
  size_t path_len;
  size_t query_len;
  size_t fragment_len;

  /* Flags */
  int is_hierarchical;
};


/* for time_t */
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

/* parsedate.c */
#ifdef HAVE_INN_PARSEDATE
#include <libinn.h>
#define RAPTOR_PARSEDATE_FUNCTION parsedate
#else
#ifdef HAVE_RAPTOR_PARSE_DATE
time_t raptor_parse_date(const char *p, time_t *now);
#define RAPTOR_PARSEDATE_FUNCTION raptor_parse_date
#else
#ifdef HAVE_CURL_CURL_H
#include <curl/curl.h>
#define RAPTOR_PARSEDATE_FUNCTION curl_getdate
#endif
#endif
#endif

/* only used internally now */
typedef void (*raptor_simple_message_handler)(void *user_data, const char *message, ...);


/* turtle_common.c */
int raptor_stringbuffer_append_turtle_string(raptor_stringbuffer* stringbuffer, const unsigned char *text, size_t len, int delim, raptor_simple_message_handler error_handler, void *error_data);


/* raptor_abbrev.c */

typedef struct {
  raptor_world* world;
  int ref_count;         /* count of references to this node */
  int count_as_subject;  /* count of this blank/resource node as subject */
  int count_as_object;   /* count of this blank/resource node as object */
  
  raptor_term* term;
} raptor_abbrev_node;


typedef struct {
  raptor_abbrev_node* node;      /* node representing the subject of
                                  * this resource */
  raptor_abbrev_node* node_type; /* the rdf:type of this resource */
  raptor_avltree *properties;    /* list of properties
                                  * (predicate/object pair) of this
                                  * subject */
  raptor_sequence *list_items;   /* list of container elements if
                                  * is rdf container */
  int valid;                     /* set 0 for blank nodes that do not
                                  * need to be referred to again */
} raptor_abbrev_subject;


raptor_abbrev_node* raptor_new_abbrev_node(raptor_world* world, raptor_term* term);
void raptor_free_abbrev_node(raptor_abbrev_node* node);
int raptor_abbrev_node_compare(raptor_abbrev_node* node1, raptor_abbrev_node* node2);
int raptor_abbrev_node_equals(raptor_abbrev_node* node1, raptor_abbrev_node* node2);
raptor_abbrev_node* raptor_abbrev_node_lookup(raptor_avltree* nodes, raptor_term* term, int* created_p);

raptor_abbrev_subject* raptor_new_abbrev_subject(raptor_abbrev_node* node);
void raptor_free_abbrev_subject(raptor_abbrev_subject* subject);
int raptor_abbrev_subject_add_property(raptor_abbrev_subject* subject, raptor_abbrev_node* predicate, raptor_abbrev_node* object);
int raptor_abbrev_subject_compare(raptor_abbrev_subject* subject1, raptor_abbrev_subject* subject2);
raptor_abbrev_subject* raptor_abbrev_subject_find(raptor_avltree *subjects, raptor_term* node);
raptor_abbrev_subject* raptor_abbrev_subject_lookup(raptor_avltree* nodes, raptor_avltree* subjects, raptor_avltree* blanks, raptor_term* term, int* created_p);
int raptor_abbrev_subject_valid(raptor_abbrev_subject *subject);
int raptor_abbrev_subject_invalidate(raptor_abbrev_subject *subject);

unsigned char *raptor_unique_id(unsigned char *base);

raptor_qname* raptor_new_qname_from_resource(raptor_sequence* namespaces, raptor_namespace_stack* nstack, int* namespace_count, raptor_abbrev_node* node);


/**
 * raptor_turtle_writer:
 *
 * Raptor Turtle Writer class
 */
typedef struct raptor_turtle_writer_s raptor_turtle_writer;

/* Turtle Writer Class (raptor_turtle_writer) */
raptor_turtle_writer* raptor_new_turtle_writer(raptor_world* world, raptor_uri* base_uri, int write_base_uri, raptor_namespace_stack *nstack, raptor_iostream* iostr);
void raptor_free_turtle_writer(raptor_turtle_writer* turtle_writer);
void raptor_turtle_writer_raw(raptor_turtle_writer* turtle_writer, const unsigned char *s);
void raptor_turtle_writer_raw_counted(raptor_turtle_writer* turtle_writer, const unsigned char *s, unsigned int len);
void raptor_turtle_writer_namespace_prefix(raptor_turtle_writer* turtle_writer, raptor_namespace* ns);
void raptor_turtle_writer_base(raptor_turtle_writer* turtle_writer, raptor_uri* base_uri);
void raptor_turtle_writer_increase_indent(raptor_turtle_writer *turtle_writer);
void raptor_turtle_writer_decrease_indent(raptor_turtle_writer *turtle_writer);
void raptor_turtle_writer_newline(raptor_turtle_writer *turtle_writer);
void raptor_turtle_writer_reference(raptor_turtle_writer* turtle_writer, raptor_uri* uri);
int raptor_turtle_writer_literal(raptor_turtle_writer* turtle_writer, raptor_namespace_stack *nstack, const unsigned char *s, const unsigned char* lang, raptor_uri* datatype);
void raptor_turtle_writer_qname(raptor_turtle_writer* turtle_writer, raptor_qname* qname);
int raptor_turtle_writer_quoted_counted_string(raptor_turtle_writer* turtle_writer, const unsigned char *s, size_t length);
void raptor_turtle_writer_comment(raptor_turtle_writer* turtle_writer, const unsigned char *s);
int raptor_world_enumerate_turtle_writer_options(raptor_world* world, const raptor_option option, const char **name,  raptor_uri **uri, const char **label);
int raptor_turtle_writer_set_option(raptor_turtle_writer *turtle_writer, raptor_option option, int value);
int raptor_turtle_writer_set_option_string(raptor_turtle_writer *turtle_writer, raptor_option option, const unsigned char *value);
int raptor_turtle_writer_get_option(raptor_turtle_writer *turtle_writer, raptor_option option);
const unsigned char *raptor_turtle_writer_get_option_string(raptor_turtle_writer *turtle_writer, raptor_option option);


/**
 * raptor_json_writer:
 *
 * Raptor JSON Writer class
 */
typedef struct raptor_json_writer_s raptor_json_writer;

/* raptor_json_writer.c */
raptor_json_writer* raptor_new_json_writer(raptor_world* world, raptor_uri* base_uri, raptor_iostream* iostr);
void raptor_free_json_writer(raptor_json_writer* json_writer);

int raptor_json_writer_newline(raptor_json_writer* json_writer);
int raptor_json_writer_key_value(raptor_json_writer* json_writer, const char* key, size_t key_len, const char* value, size_t value_len);
int raptor_json_writer_start_block(raptor_json_writer* json_writer, char c);
int raptor_json_writer_end_block(raptor_json_writer* json_writer, char c);
int raptor_json_writer_literal_object(raptor_json_writer* json_writer, unsigned char* s, unsigned char* lang, raptor_uri* datatype, const char* key, const char* type_key);
int raptor_json_writer_blank_object(raptor_json_writer* json_writer, const unsigned char* blank);
int raptor_json_writer_uri_object(raptor_json_writer* json_writer, raptor_uri* uri);
int raptor_json_writer_key_uri_value(raptor_json_writer* json_writer, const char* key, size_t key_len, raptor_uri* uri);

/* raptor_memstr.c */
const char* raptor_memstr(const char *haystack, size_t haystack_len, const char *needle);

/* raptor_serialize_rdfxmla.c special functions for embedding rdf/xml */
int raptor_rdfxmla_serialize_set_write_rdf_RDF(raptor_serializer* serializer, int value);
int raptor_rdfxmla_serialize_set_xml_writer(raptor_serializer* serializer, raptor_xml_writer* xml_writer, raptor_namespace_stack *nstack);
int raptor_rdfxmla_serialize_set_single_node(raptor_serializer* serializer, raptor_uri* uri);
int raptor_rdfxmla_serialize_set_write_typed_nodes(raptor_serializer* serializer, int value);

/* snprintf.c */
char* raptor_format_float(char *buffer, size_t *currlen, size_t maxlen, double fvalue, unsigned int min, unsigned int max, int flags);

/* raptor_world structure */
struct raptor_world_s {
  /* world has been initialized with raptor_world_open() */
  int opened;

  /* internal flag used to ignore errors for e.g. child GRDDL parsers */
  int internal_ignore_errors;
  
  raptor_error_handlers error_handlers;

  /* sequence of parser factories */
  raptor_sequence *parsers;

  /* sequence of serializer factories */
  raptor_sequence *serializers;

  /* raptor_rss_common initialisation counter */
  int rss_common_initialised;

  /* raptor_rss_{namespaces,types,fields}_info const data initialized to raptor_uri,raptor_qname objects */
  raptor_uri **rss_namespaces_info_uris;
  raptor_uri **rss_types_info_uris;
  raptor_qname **rss_types_info_qnames;
  raptor_uri **rss_fields_info_uris;
  raptor_qname **rss_fields_info_qnames;

  /* raptor_www v2 flags */
  int www_skip_www_init_finish;
  int www_initialized;

  /* raptor_sax2 init counter to work around issues in xml parser init/cleanup */
  int sax2_initialized;

  /* This is used to store a #xsltSecurityPrefsPtr typed object
   * pointer when libxslt is compiled in.
   */
  void* xslt_security_preferences;
  /* If non-0 - raptors own the above object and should free it with
   * xsltFreeSecurityPrefs() on exit
   */
  int free_xslt_security_preferences;

  /* Flags for libxml set by raptor_world_set_libxml_flags().
   * See #raptor_libxml_flags for meanings 
   */
  int libxml_flags;

#ifdef RAPTOR_XML_LIBXML
  void *libxml_saved_structured_error_context;
  xmlStructuredErrorFunc libxml_saved_structured_error_handler;
  
  void *libxml_saved_generic_error_context;
  xmlGenericErrorFunc libxml_saved_generic_error_handler;
#endif  

  raptor_avltree *uris_tree;

  raptor_uri* concepts[RDF_NS_LAST + 1];

  raptor_term* terms[RDF_NS_LAST + 1];
};

/* end of RAPTOR_INTERNAL */
#endif


#ifdef __cplusplus
}
#endif

#endif
