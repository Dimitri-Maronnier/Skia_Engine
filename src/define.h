#ifndef DEFINE_H
#define DEFINE_H

#define SAFE_DELETE( p )       { if( p!=nullptr ) { delete ( p );     ( p ) = nullptr; } }
#define IN_EDITOR 1

#endif // DEFINE_H
