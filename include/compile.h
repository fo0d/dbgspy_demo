/* ********************************************************* *
 * By Yuriy Y. Yermilov aka (binaryONE) cyclone.yyy@gmail.com
 *
 * website: code.computronium.io
 *
 * THIS SOFTWARE IS PROVIDED BY THE OWNER ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE OWNER
 * PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ********************************************************** */

//
// Order matters.
//

void
y3_dbgspy(struct y3_dbg* T);

void
run_dbgspy_default_test();
void
run_fib_test();
void
fib_viz_output(struct y3_dbg* T, struct y3_dbg_target* my_targets[]);
void
run_dbgspy_fib_test();