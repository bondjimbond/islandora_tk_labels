jQuery(document).ready(function() {
  jQuery(".tk_icon").on("click", function(e) {
    jQuery(e.currentTarget).next(".lc_textblock").toggle();
  })
})
