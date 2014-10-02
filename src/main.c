/*
 *                     :::::::: ::::::::::: :::    ::: ::::    :::
 *                   :+:    :+:    :+:     :+:    :+: :+:+:   :+:
 *                  +:+           +:+     +:+    +:+ :+:+:+  +:+
 *                 +#++:++#++    +#+     +#+    +:+ +#+ +:+ +#+
 *                       +#+    +#+     +#+    +#+ +#+  +#+#+#
 *               #+#    #+#    #+#     #+#    #+# #+#   #+#+#
 *               ########     ###      ########  ###    ####
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 2.0 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *
 *        http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an AS IS basis,
 * without warranty of any kind, either express or implied.  See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 3 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 3 or later (the "LGPL"), in
 * which case the provisions of the GPL or the LGPL are applicable instead of
 * those above.
 *
 * If you wish to allow use of your version of this file only under the terms
 * of either the GPL or the LGPL, and not to allow others to use your version
 * of this file under the terms of the MPL, indicate your decision by deleting
 * the provisions above and replace them with the notice and other provisions
 * required by the GPL or the LGPL. If you do not delete the provisions above,
 * a recipient may use your version of this file under the terms of any one of
 * the MPL, the GPL or the LGPL. */

#include "stun.h"

int main(int count, char **arguments)
{
	layer_t *temporary[2] = { NULL, NULL };
	layer_t *simplex = NULL;

	window(1280, 720, WINDOWED);

	glEnable(GL_DEPTH_TEST);

	temporary[0] = layer(WINDOW_WIDTH, WINDOW_HEIGHT);
	temporary[1] = layer(WINDOW_WIDTH, WINDOW_HEIGHT);

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		perspective(WINDOW_WIDTH, WINDOW_HEIGHT, 1.77777778f);
		glTranslatef(0.f, 0.f, -1.f);

		bind_accumulation_layer();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Render your stuff here, it all goes into the accumulation
		 * layer; this is done so that after the completion of your
		 * render operations you can easily apply post-processing
		 * effects and filters to your render product. */

		blit_accumulator_to_layer(ACCUMULATION, temporary[0]);

		/* Here's an example of a post-processing filter, applying
		 * Gaussian Blur to the contents of the accumulation buffer. */
		gaussian_blur(temporary[0], temporary[1]);

		/* Finally, blit everything to back-buffer! */
		blit_to_back_buffer(temporary[0]);

		flush();
	} while (!escape());

	quit();

	/* The sun is setting in the sky, Teletubbies say goodbye! */
	return EXIT_SUCCESS;
}
