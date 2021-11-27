#include <iostream>

#include "archs/intel/intel_arch.h"

using namespace std;

int main() {

	ProgramHolder ph;
	auto code_section = ph.addSection(SEC::CODE);
	ph.setSection(code_section);

	LABEL start = ph.createLabel();
	LABEL end = ph.createLabel();

	ph.setLabelHere(start);

	ph.add_al(end);

	ph.setLabelHere(end);


	ph.compileSections();
	ph.logSection(code_section);

	return 0;
}