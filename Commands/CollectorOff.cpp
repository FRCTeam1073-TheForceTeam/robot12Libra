#include "CollectorOff.h"
CollectorOff::CollectorOff() {
	Requires(collector1);
}
void CollectorOff::Initialize() {
	collector1->AllMotorsOff();
}
void CollectorOff::Execute() {}
bool CollectorOff::IsFinished() {
	return true;
}
void CollectorOff::End() {}
void CollectorOff::Interrupted() {}
