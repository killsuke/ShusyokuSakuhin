#include "Singleton.h"
#include <array>
#include <iostream>

namespace mozc {
	namespace internal {
		namespace {
			std::array<void (*)(void), 256> finalizers;	// “o˜^”§ŒÀ
			int size = 0;	// “o˜^”‡”Ô
		}

		// ƒVƒ“ƒOƒ‹ƒgƒ“‚ğì¬ŒãAíœ‘ÎÛ‚Æ‚µ‚Ä“o˜^
		void AddSingletonFinalizer(void (*finalizer)()) {
			if (size >= finalizers.size()) {
				std::cout << "ƒVƒ“ƒOƒ‹ƒgƒ““o˜^”ƒGƒ‰[I" << std::endl;
			}
			finalizers[size++] = finalizer;	// Œã’uƒCƒ“ƒNƒŠƒƒ“ƒg‚È‚Ì‚ÅAƒ[ƒ‚©‚çn‚Ü‚é
		}

	}

	// “o˜^‚³‚ê‚½‹t‡‚Åíœ
	void FinalizeSingletons() {
		for (int i = internal::size - 1; i >= 0; --i) {
			internal::finalizers[i]();
		}
		internal::size = 0;	// ƒŠƒZƒbƒg
	}
}