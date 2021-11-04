declare <2 x i32> @llvm.x86.mmx.punpckhdq(<2 x i32>, <2 x i32>)

define <2 x i32> @src(<2 x i32> %a) {
  %1 = call <2 x i32> @llvm.x86.mmx.punpckhdq(<2 x i32> %a, <2 x i32> poison)
  ret <2 x i32> %1
}

define <2 x i32> @tgt(<2 x i32> %a) {
  %tmp = shufflevector <2 x i32> %a, <2 x i32> poison, <2 x i32> <i32 1, i32 3>
  ret <2 x i32> %tmp
}
