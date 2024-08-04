#include "Core.h"
#include "Core/utils.h"
#include "Core/enum.h"
#include "Core/globalFunc.h"
#include "Core/type.h"
#include "Core/value.h"
#include "Core/miscClasses.h"
#include "Core/iterator.h"


void populateCore(nanobind::module_ &m) {
  bindEnums(m);
  bindGlobalFunctions(m);
  bindTypeClasses(m);
  bindValueClasses(m);
  bindOtherClasses(m);
  bindIterators(m);
}


