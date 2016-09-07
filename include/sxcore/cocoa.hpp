#pragma once
SXMODULE_SXCORE

#if SXAPI_COCOA || SXAPI_COCOA_TOUCH
	#ifndef NSINTEGER_DEFINED
		#if __LP64__
			using NSInteger		= long;
			using NSUInteger	=  unsigned long;
		#else
			using NSInteger		= int;
			using NSUInteger	= unsigned int;
		#endif
	#endif

	namespace sx {
		class autoid {
		public:
			id p;

			explicit autoid (id p = nil) : p(p) { }
			~autoid () {
				if (p) [p release];
			}
			operator id () const {
				return p;
			}
		};
		
//		class autoreleasepool {
//		public:
//			autoreleasepool () : pool(nil) {
//				pool = [[NSAutoreleasePool alloc] init];
//			}
//			~autoreleasepool () {
//				[pool release];
//			}
//			void release () {
//				[pool release];
//				pool = nil;
//			}
//		private:
//			NSAutoreleasePool *pool;
//		};
//
		void raise_exception ();
	}
//#else
//	namespace sx {
//		class autoreleasepool {
//		};
//	}
#endif
	
#if SXAPI_COCOA
	inline std::ostream& operator<< (std::ostream &o, const NSRect &r) {
		o << r.origin.x << " " << r.origin.y << " " << r.size.width << " " << r.size.height;
		return o;
	}
#endif

#define SXCOCOAIMPLEMENTATION do { sxassert(true); } while (false)
