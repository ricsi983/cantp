#include "CanInterface.h"
#include "CanTpConstants.h"
#include "FrameSender.h"
#include "MultiFrameBuilder.h"
#include "SingleFrameBuilder.h"
#include "Utils.h"
#include <memory>
#include <stdlib.h>
#include <variant>
#include <vector>

template <typename T> class CanTp
{
  using BuilderVariant
      = std::variant<SingleFrameBuilder<T>, MultiFrameBuilder<T> >;

private:
  CanInterface &_interface;

public:
  CanTp (CanInterface &interface) : _interface (interface) {}

  void
  SendMessage (uint8_t *payload, uint32_t length, uint32_t source,
               uint32_t destination)
  {
    MessageHandle handle = { payload, length, source, destination };
    BuilderVariant builder;
    builder = (CanUtils::IsSingleFrame<T> (length)
                   ? BuilderVariant{ SingleFrameBuilder<T> () }
                   : BuilderVariant{ MultiFrameBuilder<T> () });
    std::visit (FrameSender<T> (_interface, handle), builder);
  }

  void
  ReceiveMessage (uint8_t *payload, uint32_t &length, uint32_t source,
                  uint32_t destination)
  {
  }
};