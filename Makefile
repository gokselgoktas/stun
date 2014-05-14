#
#                     :::::::: ::::::::::: :::    ::: ::::    :::
#                   :+:    :+:    :+:     :+:    :+: :+:+:   :+:
#                  +:+           +:+     +:+    +:+ :+:+:+  +:+
#                 +#++:++#++    +#+     +#+    +:+ +#+ +:+ +#+
#                       +#+    +#+     +#+    +#+ +#+  +#+#+#
#               #+#    #+#    #+#     #+#    #+# #+#   #+#+#
#               ########     ###      ########  ###    ####
#
# The contents of this file are subject to the Mozilla Public License Version
# 1.1 (the "License"); you may not use this file except in compliance with the
# License. You may obtain a copy of the License at:
#
#        http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an AS IS basis,
# without warranty of any kind, either express or implied.  See the License for
# the specific language governing rights and limitations under the License.
#
# Alternatively, the contents of this file may be used under the terms of
# either the GNU General Public License Version 3 or later (the "GPL"), or the
# GNU Lesser General Public License Version 3 or later (the "LGPL"), in which
# case the provisions of the GPL or the LGPL are applicable instead of those
# above.
#
# If you wish to allow use of your version of this file only under the terms of
# either the GPL or the LGPL, and not to allow others to use your version of
# this file under the terms of the MPL, indicate your decision by deleting the
# provisions above and replace them with the notice and other provisions
# required by the GPL or the LGPL. If you do not delete the provisions above, a
# recipient may use your version of this file under the terms of any one of the
# MPL, the GPL or the LGPL.

TARGET = bin/stun
OBJ = src/main.o src/stun.o

FRAGMENT = $(wildcard src/*.fragment)
VERTEX = $(wildcard src/*.vertex)

INL = $(FRAGMENT:.fragment=.fragment.inl) $(VERTEX:.vertex=.vertex.inl)

LIBS = libglfw glew gl assimp
CLEAN = $(TARGET) $(OBJ) $(DEPS) $(INL)
CC = clang

CFLAGS += -Iinclude `pkg-config --cflags $(LIBS)`
LDFLAGS += `pkg-config --libs $(LIBS)` -lm

DEPS = $(OBJ:%.o=%.d)

all: $(TARGET)

-include $(DEPS)

$(TARGET): $(INL) $(OBJ)
	@mkdir -p bin
	@$(CC) $(OBJ) $(LDFLAGS) -o $@
	@-rm -f $(OBJ) $(DEPS) $(INL)

.PHONY: clean
clean:
	@-rm -f $(CLEAN)

run:
	@bin/monolith

%.o: %.c
	@$(CC) -MMD -c $(CFLAGS) $< -o $@

%.fragment.inl: %.fragment
	@sed 's/^/"/;s/$$/"/;s/["][[:blank:]]\+/        "/' $< > $@
	@echo "," >> $@

%.vertex.inl: %.vertex
	@sed 's/^/"/;s/$$/"/;s/["][[:blank:]]\+/        "/' $< > $@
	@echo "," >> $@
