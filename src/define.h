#ifndef DEFINE_H
#define DEFINE_H

#define SAFE_DELETE( p )       { if( p ) { delete ( p );     ( p ) = NULL; } }

#endif // DEFINE_H
