#pragma once
SXMODULE_SXCORE
#include "sxcore/endian.hpp"
#include "sxcore/debug.hpp"

namespace sx {
	/// \brief \en uuid_class \enden \ja UUIDを扱うクラス \endja
	///
	/// \en \enden \ja UUIDの定義や、int型のプラグインIDをUUIDに変換することができる。詳細は \ref uuid を参照。 \endja
	class uuid_class {
	public:
		uuid_class () : a(0), b(0), c(0), d(0) { }
		/// \en blah \enden \ja ハイフン区切りでのUUIDを使用する場合に使う。\endja
		explicit uuid_class (const char *s) {
			set(s);
		}
#ifdef OBSOLETE_USE_DETECTOR
#pragma message("DETECTING POTENTIAL ERROR OBSOLETE CONSTRUCTOR FOR uuid_class, fix error C2440.")
#else
		explicit uuid_class(std::uint32_t a) : a(a), b(0), c(0), d(0) {
		}
#endif
		explicit uuid_class (std::uint32_t a, std::uint32_t b, std::uint32_t c, std::uint32_t d) : a(a), b(b), c(c), d(d) { }
		#if SXAPI_COCOA || SXAPI_COCOA_TOUCH
			explicit uuid_class (const CFUUIDRef uuid) { set(uuid); }
		#endif
        #if SXAPI_CORE_WIN32
			explicit uuid_class (const UUID &uuid) { set(uuid); }
		#endif
		bool operator< (const uuid_class &b) const {
			const uuid_class &a = *this;
			if (a.a < b.a)		return true;
			else if (b.a < a.a)	return false;
			if (a.b < b.b)		return true;
			else if (b.b < a.b)	return false;
			if (a.c < b.c)		return true;
			else if (b.c < a.c)	return false;
			if (a.d < b.d)		return true;
			else if (b.d < a.d)	return false;
			return false;
		}
		/// \brief \en blah \enden \ja uuidが同じかどうか判別する。 \endja
		bool operator== (const uuid_class &t) const {
			return (a == t.a) && (b == t.b) && (c == t.c) && (d == t.d);
		}
		bool operator!= (const uuid_class &t) const {
			return !((*this) == t);
		}
		/// \brief \en blah \enden \ja UUIDの先頭の32bitを返す。\endja
		std::uint32_t n () const {
			if ((b == 0) && (c == 0) && (d == 0))	return a;
			return 0xffffffff;
		}
		bool is_enum () const {
			return ((b == 0) && (c == 0) && (d == 0));
		}
		/// \en blah \enden \ja ハイフン区切りのUUIDをstringで返す。\endja
		std::string string () const;
		void revert_endian () {
			sx::revert_endian(a);
			sx::revert_endian(b);
			sx::revert_endian(c);
			sx::revert_endian(d);
		}
	private:
		std::uint32_t a{ 0 };
		std::uint32_t b{ 0 };
		std::uint32_t c{ 0 };
		std::uint32_t d{ 0 };

		#if SXAPI_CORE_FOUNDATION
			void set (const CFUUIDRef uuid);
		#endif
		#if SXAPI_CORE_WIN32
			void set (const UUID &uuid);
		#endif
		void set (const char *s) {
			SXASSERT(s);
			a = get8(s);
			s += 9;
			b = (get4(s)<<16) + get4(s+5);
			s += 10;
			c = (get4(s)<<16) + get4(s+5);
			s += 9;
			d = get8(s);
		}
		void set (std::uint32_t a, std::uint32_t b, std::uint32_t c, std::uint32_t d) {
			this->a = a; this->b = b; this->c = c; this->d = d;
		}
		std::uint32_t get8 (const char *s) {
			return (get1(s[0])<<28) + (get1(s[1])<<24) + (get1(s[2])<<20) + (get1(s[3])<<16) + (get1(s[4])<<12) + (get1(s[5])<<8) + (get1(s[6])<<4) + (get1(s[7])<<0);
		}
		std::uint32_t get4 (const char *s) {
			return (get1(s[0])<<12) + (get1(s[1])<<8) + (get1(s[2])<<4) + (get1(s[3])<<0);
		}
		std::uint32_t get1 (char s) {
			switch (s) {
			case '0':			return 0x00;
			case '1':			return 0x01;
			case '2':			return 0x02;
			case '3':			return 0x03;
			case '4':			return 0x04;
			case '5':			return 0x05;
			case '6':			return 0x06;
			case '7':			return 0x07;
			case '8':			return 0x08;
			case '9':			return 0x09;
			case 'A': case 'a':	return 0x0A;
			case 'B': case 'b':	return 0x0B;
			case 'C': case 'c':	return 0x0C;
			case 'D': case 'd':	return 0x0D;
			case 'E': case 'e':	return 0x0E;
			case 'F': case 'f':	return 0x0F;
			}
			return 0;
		}
	};
	
	/// \brief \en blah \enden \ja 新規のUUIDを生成する。 \endja
	uuid_class create_uuid ();

	template<> inline void revert_endian (sx::uuid_class &uuid) {
		uuid.revert_endian();
	}
}

