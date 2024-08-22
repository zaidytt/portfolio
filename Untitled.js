$(document).ready(function() {
  console.log("Document is ready");

  // Add smooth scrolling to all links
  $("a").on('click', function(event) {
    console.log("Link clicked: ", this.hash);

    // Make sure this.hash has a value before overriding default behavior
    if (this.hash !== "") {
      console.log("Hash is not empty");

      // Prevent default anchor click behavior
      event.preventDefault();

      // Store hash
      var hash = this.hash;

      // Using jQuery's animate() method to add smooth page scroll
      // The optional number (800) specifies the number of milliseconds it takes to scroll to the specified area
      $('html, body').animate({
        scrollTop: $(hash).offset().top
      }, 800, function() {
        // Add hash (#) to URL when done scrolling (default click behavior)
        window.location.hash = hash;

        console.log("Finished scrolling to: ", hash);

        // Enable vertical scrolling and disable horizontal scrolling when navigating to page1
        if (hash === "#page1") {
          console.log("Hash is #page1, enabling vertical scroll");
          enableVerticalScroll();
        }
      });
    } else {
      console.log("Hash is empty, no action taken");
    }
  });

  // Function to handle horizontal scrolling
  function horizontalScroll(ev) {
    console.log("Horizontal scroll event");
    ev.preventDefault();
    scrollableElement.scrollLeft += ev.deltaY + ev.deltaX;
    console.log("Horizontal scroll position: ", scrollableElement.scrollLeft);
  }

  // Add horizontal scroll event listener to scrollableElement
  var scrollableElement = document.getElementById('scrollableElement');
  if (scrollableElement) {
    scrollableElement.addEventListener('wheel', horizontalScroll);
    console.log("Horizontal scroll listener added to scrollableElement");
  } else {
    console.error("scrollableElement not found");
  }

  // Function to enable vertical scrolling
  function enableVerticalScroll() {
    console.log("Enabling vertical scrolling");
    $('body, html').css('overflow-y', 'auto');
    if (scrollableElement) {
      scrollableElement.removeEventListener('wheel', horizontalScroll);
      console.log("Horizontal scroll listener removed from scrollableElement");
    } else {
      console.error("scrollableElement not found for removing listener");
    }
  }

  // Function to enable horizontal scrolling and disable vertical scrolling
  function enableHorizontalScroll() {
    console.log("Enabling horizontal scrolling, disabling vertical scrolling");
    $('body, html').css('overflow-y', 'hidden');
    if (scrollableElement) {
      scrollableElement.addEventListener('wheel', horizontalScroll);
      console.log("Horizontal scroll listener added to scrollableElement");
    } else {
      console.error("scrollableElement not found for adding listener");
    }
  }

  // Check scroll position and enable horizontal scrolling if at the top
  $(window).on('scroll', function() {
    var scrollTop = $(window).scrollTop();
    console.log("Window scroll event, scrollTop: ", scrollTop);
    if (scrollTop <= 0) {
      console.log("Scroll position at the top, enabling horizontal scroll");
      enableHorizontalScroll();
    } else if (window.location.hash === "#page1") {
      console.log("Scroll position not at the top and hash is #page1, enabling vertical scroll");
      enableVerticalScroll();
    }
  });

  // Initial check in case the page loads with #page1
  if (window.location.hash === "#page1") {
    console.log("Initial load with hash #page1, enabling vertical scroll");
    enableVerticalScroll();
  }

  // Add initial scroll position check
  var initialScrollTop = $(window).scrollTop();
  console.log("Initial scroll position check, scrollTop: ", initialScrollTop);
  if (initialScrollTop <= 0) {
    console.log("Initial scroll position at the top, enabling horizontal scroll");
    enableHorizontalScroll();
  }
});
